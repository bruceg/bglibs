/* sig_catch.c - Cause a signal to be handled (caught) by a function
 * Copyright (C) 2001  Bruce Guenter <bruceg@em.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <signal.h>
#include "sig.h"
#include "sysdeps.h"

void sig_catch(int sig, signalfn fn)
{
#ifdef HASSIGACTION
  struct sigaction sa;
  sa.sa_handler = fn;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(sig, &sa, 0);
#else
  signal(sig,fn); /* won't work under System V, even nowadays---dorks */
#endif
}

void sig_default(int sig) { sig_catch(sig, SIG_DFL); }
void sig_ignore(int sig) { sig_catch(sig, SIG_IGN); }
