/* $Id$ */
/* cdb/get.c -- Retrieve a record from a CDB as a str
 * Copyright (C) 2001-2002  Bruce Guenter <bruceg@em.ca>
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
#include "cdb.h"
#include "str.h"
#include "str/str.h"

int cdb_get(struct cdb* cdb, const str* key, str* data)
{
  if (cdb == 0 || key == 0 || data == 0) return -1;
  if (!cdb_find(cdb, key->s, key->len)) return 0;
  if (!str_ready(data, cdb->dlen)) return -1;
  if (cdb_read(cdb, data->s, cdb->dlen, cdb->dpos) == -1) return -1;
  data->len = cdb->dlen;
  data->s[data->len] = 0;
  return 1;
}
