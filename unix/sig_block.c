/* $Id$ */
/* sig_block.c - Block and unblock signals
 * Copyright (C) 2001  Bruce Guenter <bruce@untroubled.org>
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
#include <signal.h>
#include "sig.h"
#include "sysdeps.h"

void sig_block(int s)
{
#ifdef HASSIGPROCMASK
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, s);
  sigprocmask(SIG_BLOCK, &set, 0);
#else
  sigblock(1 << (s - 1));
#endif
}

void sig_unblock(int s)
{
#ifdef HASSIGPROCMASK
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, s);
  sigprocmask(SIG_UNBLOCK, &set, 0);
#else
  sigsetmask(sigsetmask(~0) & ~(1 << (s - 1)));
#endif
}
