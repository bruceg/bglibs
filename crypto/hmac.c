/* $Id$ */
/* crypto/hmac.c - Hashed Message Authentication Code
 * Copyright (C) 2005  Bruce Guenter <bruce@untroubled.org>
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
#include "sysdeps.h"
#include <string.h>
#include "str/str.h"
#include "hmac.h"

/** A generic RFC 2104 HMAC calculator.
 *
 * This function generates a HMAC (Keyed Hashing for Message
 * Authentication) according to RFC 2104, using a secure hash given in
 * the control block.
 */
void hmac(const struct hmac_control_block* hcb,
	  const str* secret,
	  const str* nonce,
	  void* output)
{
  unsigned char state[hcb->state_size];
  unsigned char final[hcb->final_size];
  unsigned char block[hcb->block_size];
  unsigned i;

  /* Set up K XOR ipad, where ipad is 0x36 repeated B times */
  if (secret->len >= hcb->block_size) {
    hcb->init(state);
    hcb->update(state, secret->s, secret->len);
    hcb->finalize(state, block);
    memset(block + sizeof final, 0, sizeof block - sizeof final);
  }
  else {
    memcpy(block, secret->s, secret->len);
    memset(block + secret->len, 0, sizeof block - secret->len);
  }
  for (i = 0; i < sizeof block; ++i)
    block[i] ^= 0x36;

  /* Generate H1 = H(K XOR ipad, nonce) */
  hcb->init(state);
  hcb->update(state, block, sizeof block);
  hcb->update(state, nonce->s, nonce->len);
  hcb->finalize(state, final);
  
  /* Generate K XOR opad */
  for (i = 0; i < sizeof block; ++i)
    block[i] ^= 0x36 ^ 0x5c;
  
  /* Generate Output = H(K XOR opad, H1) */
  hcb->init(state);
  hcb->update(state, block, sizeof block);
  hcb->update(state, final, sizeof final);
  hcb->finalize(state, output);
}
