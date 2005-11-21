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

static int do_show_usage = 0;
const char* argv0;
const char* argv0base;
const char* argv0dir;

static cli_option help_option = { 'h', "help", CLI_FLAG,
				  1, &do_show_usage,
				  "Display this help and exit", 0 };

static cli_option** options;
static unsigned optionc;

static void build_options()
{
  unsigned i;
  for(optionc = 0;
      cli_options[optionc].ch || cli_options[optionc].name;
      optionc++) ;
  optionc++;
  options = malloc(optionc * sizeof *options);
  for(i = 0; i < optionc-1; i++)
    options[i] = &cli_options[i];
  options[optionc-1] = &help_option;
}

static void show_usage()
{
  obuf_put5s(&outbuf, "usage: ", program, " [flags] ", cli_args_usage, "\n");
}

static unsigned calc_max_width()
{
  /* maxwidth is the maximum width of the long argument */
  unsigned maxwidth = 0;
  unsigned i;
  for(i = 0; i < optionc; i++) {
    unsigned width = 0;
    cli_option* o = options[i];
    if(o->name) {
      width += strlen(o->name);
      switch(o->type) {
      case CLI_STRING:     width += 6; break;
      case CLI_INTEGER:    width += 4; break;
      case CLI_UINTEGER:   width += 4; break;
      case CLI_STRINGLIST: width += 5; break;
      case CLI_FUNCTION:   width += 6; break;
      case CLI_SEPARATOR:  width = 0; break;
      case CLI_FLAG:       break;
      case CLI_COUNTER:    break;
      }
    }
    if(width > maxwidth)
      maxwidth = width;
  }
  return maxwidth;
}

static void show_option(cli_option* o, unsigned maxwidth)
{
  if(o->type == CLI_SEPARATOR) {
    obuf_put3s(&outbuf, "\n", o->name, ":\n");
    return;
  }
  if(o == &help_option) obuf_putc(&outbuf, '\n');
  if(o->ch) {
    obuf_puts(&outbuf, "  -");
    obuf_putc(&outbuf, o->ch);
  }
  else
    obuf_puts(&outbuf, "    ");
  obuf_puts(&outbuf, (o->ch && o->name) ? ", " : "  ");
  if(o->name) {
    const char* extra = "";
    switch(o->type) {
    case CLI_STRING:     extra = "=VALUE"; break;
    case CLI_INTEGER:    extra = "=INT"; break;
    case CLI_UINTEGER:   extra = "=UNS"; break;
    case CLI_STRINGLIST: extra = "=ITEM"; break;
    case CLI_FUNCTION:   extra = "=VALUE"; break;
    case CLI_FLAG:       break;
    case CLI_COUNTER:    break;
    case CLI_SEPARATOR:  break;
    }
    obuf_put3s(&outbuf, "--", o->name, extra);
    obuf_pad(&outbuf, maxwidth - strlen(o->name) - strlen(extra) + 2, ' ');
  }
  else
    obuf_pad(&outbuf, maxwidth+4, ' ');
  obuf_put2s(&outbuf, o->helpstr, "\n");
  if(o->defaultstr) {
    obuf_pad(&outbuf, maxwidth+10, ' ');
    obuf_put3s(&outbuf, "(Defaults to ", o->defaultstr, ")\n");
  }
}

static void show_help()
{
  unsigned maxwidth;
  unsigned i;
  obuf_puts(&outbuf, cli_help_prefix);
  maxwidth = calc_max_width();
  for(i = 0; i < optionc; i++)
    show_option(options[i], maxwidth);
  obuf_puts(&outbuf, cli_help_suffix);
}

void usage(int exit_value, const char* errorstr)
{
  if(errorstr)
    error1(errorstr);
  show_usage();
  show_help();
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
    for(j = 0; j < optionc; j++) {
      cli_option* o = options[j];
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
    if(j >= optionc) {
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
  for(j = 0; j < optionc; j++) {
    cli_option* o = options[j];
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
