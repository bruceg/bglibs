/* $Id$ */

/* hash-lookup.c - Look up a hash control block by name
 * Copyright (C) 2006  Bruce Guenter <bruce@untroubled.org>
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

#include <string.h>
#include "hash.h"

#define TRY(NAME) if (strcasecmp(name,hash_##NAME .name) == 0) return &hash_##NAME

/** Look up a hash control block by name.
 *
 * This function iterates through all known hash control blocks,
 * returning the first one that has a name that matches the given name
 * (case insensitive).
 *
 * \returns a pointer to the first matching control block, or \c NULL.
 */
const struct hash_control_block* hash_lookup(const char* name)
{
  TRY(md4);
  TRY(md5);
  TRY(sha1);
  TRY(sha256);
  TRY(sha384);
  TRY(sha512);
  return 0;
}
