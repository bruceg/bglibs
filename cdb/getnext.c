/* cdb/strgetnext.c -- Retrieve the next record from a CDB as a str
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
#include "cdb.h"
#include "str.h"

int cdb_getnext(struct cdb* cdb, const str* key, str* data)
{
  if (cdb == 0 || key == 0 || data == 0) return -1;
  if (!cdb_findnext(cdb, key->s, key->len)) return 0;
  if (!str_ready(data, cdb->dlen)) return -1;
  if (cdb_read(cdb, (unsigned char*)data->s, cdb->dlen, cdb->dpos) == -1)
    return -1;
  data->len = cdb->dlen;
  data->s[data->len] = 0;
  return 1;
}
