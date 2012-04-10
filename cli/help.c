/* cli/help.c - Show dynamic help text
 * Copyright (C) 2001,2005,2006  Bruce Guenter <bruce@untroubled.org>
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

#include "iobuf.h"
#include "msg.h"
#include <time.h>
#include "systime.h"
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "internal.h"

static unsigned calc_max_width()
{
  /* maxwidth is the maximum width of the long argument */
  unsigned maxwidth = 0;
  unsigned i;
  for(i = 0; i < cli_option_count; i++) {
    unsigned width = 0;
    cli_option* o = cli_full_options[i];
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
  if(o == &cli_help_option) obuf_putc(&outbuf, '\n');
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

void cli_show_help(void)
{
  unsigned maxwidth;
  unsigned i;
  obuf_puts(&outbuf, cli_help_prefix);
  maxwidth = calc_max_width();
  for(i = 0; i < cli_option_count; i++)
    show_option(cli_full_options[i], maxwidth);
  obuf_puts(&outbuf, cli_help_suffix);
}
