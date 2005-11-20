/* $Id$ */
/* sig_block.c - Block and unblock signals
 * Copyright (C) 2001  Bruce Guenter <bruce@untroubled.org>
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
