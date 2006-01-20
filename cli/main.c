/* $Id$ */
/* cli/main.c - Main routine for all CLI programs
 * Copyright (C) 2001,2005  Bruce Guenter <bruce@untroubled.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include "iobuf/iobuf.h"
#include "msg/msg.h"
#include <time.h>
#include "systime.h"
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "internal.h"

static int do_show_usage = 0;
const char* argv0;
const char* argv0base;
const char* argv0dir;

cli_option cli_help_option = { 'h', "help", CLI_FLAG,
			       1, &do_show_usage,
			       "Display this help and exit", 0 };

cli_option** cli_full_options;
unsigned cli_option_count;

static void build_options()
{
  unsigned i;
  for(cli_option_count = 0;
      cli_options[cli_option_count].ch || cli_options[cli_option_count].name;
      cli_option_count++) ;
  cli_option_count++;
  cli_full_options = malloc(cli_option_count * sizeof *cli_full_options);
  for(i = 0; i < cli_option_count-1; i++)
    cli_full_options[i] = &cli_options[i];
  cli_full_options[cli_option_count-1] = &cli_help_option;
}

static void show_usage()
{
  obuf_put5s(&outbuf, "usage: ", program, " [flags] ", cli_args_usage, "\n");
}

void usage(int exit_value, const char* errorstr)
{
  if(errorstr)
    error1(errorstr);
  show_usage();
  cli_show_help();
  obuf_flush(&outbuf);
  exit(exit_value);
}

static cli_stringlist* stringlist_append(cli_stringlist* node,
					 const char* newstr,
					 const cli_option* option)
{
  cli_stringlist* newnode;
  newnode = malloc(sizeof *newnode);
  newnode->string = newstr;
  newnode->option = option;
  newnode->next = 0;
  if(node) {
    cli_stringlist* head = node;
    while(node->next)
      node = node->next;
    node->next = newnode;
    return head;
  }
  else
    return newnode;
}

static int cli_option_set(cli_option* o, const char* arg)
{
  char* endptr;
  switch(o->type) {
  case CLI_FLAG:
    *(int*)o->dataptr = o->flag_value;
    return 0;
  case CLI_COUNTER:
    *(int*)o->dataptr += o->flag_value;
    return 0;
  case CLI_INTEGER:
    *(int*)o->dataptr = strtol(arg, &endptr, 10);
    if(*endptr) {
      error2("Invalid integer: ", arg);
      return -1;
    }
    return 1;
  case CLI_UINTEGER:
    *(unsigned*)o->dataptr = strtoul(arg, &endptr, 10);
    if(*endptr) {
      error2("Invalid unsigned integer: ", arg);
      return -1;
    }
    return 1;
  case CLI_STRINGLIST:
    *(cli_stringlist**)o->dataptr =
      stringlist_append(*(cli_stringlist**)o->dataptr, arg, o);
    return 1;
  case CLI_FUNCTION:
    ((cli_function*)o->dataptr)(arg, o);
    return 1;
  default:
    *(const char**)o->dataptr = arg;
    return 1;
  }
}

static int parse_short(int argc, char* argv[])
{
  int i;
  int end = strlen(argv[0]) - 1;
  for(i = 1; i <= end; i++) {
    int ch = argv[0][i];
    unsigned j;
    for(j = 0; j < cli_option_count; j++) {
      cli_option* o = cli_full_options[j];
      if(o->ch == ch) {
	if(o->type != CLI_FLAG &&
	   o->type != CLI_COUNTER) {
	  if(i < end) {
	    if(cli_option_set(o, argv[0]+i+1) != -1)
	      return 0;
	  }
	  else if(argc <= 1) {
	    char tmp[] = "Option -X requires a value.";
	    tmp[8] = o->ch;
	    error1(tmp);
	  }
	  else
	    if(cli_option_set(o, argv[1]) != -1)
	      return 1;
	}
	else if(cli_option_set(o, 0) != -1)
	  break;
	return -1;
      }
    }
    if(j >= cli_option_count) {
      char tmp[] = "Unknown option letter -X.";
      tmp[23] = argv[0][i];
      error1(tmp);
      return -1;
    }
  }
  return 0;
}

static int cli_option_parse_long_eq(cli_option* o, const char* arg)
{
  if(o->type == CLI_FLAG || o->type == CLI_COUNTER) {
    error3("Option --", o->name, " does not take a value.");
    return -1;
  }
  else
    return cli_option_set(o, arg) - 1;
}

static int cli_option_parse_long_noeq(cli_option* o, const char* arg)
{
  if(o->type == CLI_FLAG || o->type == CLI_COUNTER)
    return cli_option_set(o, 0);
  else if(arg)
    return cli_option_set(o, arg);
  else {
    error3("Option --", o->name, " requires a value.");
    return -1;
  }
}

static int parse_long(char* argv[])
{
  unsigned j;
  const char* arg = argv[0]+2;
  for(j = 0; j < cli_option_count; j++) {
    cli_option* o = cli_full_options[j];
    if(o->name) {
      size_t len = strlen(o->name);
      if(!memcmp(arg, o->name, len)) {
	if(arg[len] == '\0')
	  return cli_option_parse_long_noeq(o, argv[1]);
	else if(arg[len] == '=')
	  return cli_option_parse_long_eq(o, arg+len+1);
      }
    }
  }
  error3("Unknown option string: '--", arg, "'");
  return -1;
}

static int parse_args(int argc, char* argv[])
{
  int i;
  int j;
  build_options();
  for(i = 1; i < argc; i++) {
    const char* arg = argv[i];
    // Stop at the first non-option argument
    if(arg[0] != '-')
      break;
    // Stop after the first "-" or "--"
    if(arg[1] == '\0' ||
       (arg[1] == '-' && arg[2] == '\0')) {
      i++;
      break;
    }
    j = (arg[1] != '-') ?
      parse_short(argc-i, argv+i) :
      parse_long(argv+i);
    if(j < 0)
      usage(1, 0);
    else
      i += j;
  }
  return i;
}

static void set_argv0(const char* p)
{
  static const char* empty = "";
  const char* s = strrchr(p, '/');
  size_t length;
  char* tmp;
  argv0 = p;
  if(s) {
    ++s;
    argv0base = s;
    length = s-p;
    tmp = malloc(length+1);
    memcpy(tmp, p, length);
    tmp[length] = 0;
    argv0dir = tmp;
  }
  else {
    argv0base = p;
    argv0dir = empty;
  }
}

int main(int argc, char* argv[])
{
  struct timeval tv;
  int lastarg;
  
  gettimeofday(&tv, 0);
  srandom(tv.tv_usec ^ tv.tv_sec);
  
  set_argv0(argv[0]);
  lastarg = parse_args(argc, argv);

  if(do_show_usage)
    usage(0, 0);

  argc -= lastarg;
  argv += lastarg;
  if(argc < cli_args_min)
    usage(1, "Too few command-line arguments");
  if(cli_args_max >= cli_args_min && argc > cli_args_max)
    usage(1, "Too many command-line arguments");
  
  return cli_main(argc, argv);
}
