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
#include "str.h"
#include "hmac.h"

/** Precalculate the HMAC intermediate values.
 *
 * Calculates the initialization vectors to effectively seed the hash
 * function with the result of H(K XOR ipad) and H(K XOR opad).  The
 * results of this function are used by hmac_finish.  This saves the
 * application of the hash's compression function on two blocks for each
 * use of the HMAC.
 *
 * \note \c midstate must be twice \c hcb->midstate_size bytes long.
 */
void hmac_prepare(const struct hmac_control_block* hcb,
		  void* midstate,
		  const str* secret)
{
  uint64 statebuf[(hcb->state_size + 7) / sizeof(uint64)];
  unsigned char *state = (unsigned char *)statebuf;
  unsigned char block[hcb->block_size];
  unsigned i;

  /* Set up K XOR ipad, where ipad is 0x36 repeated B times */
  if (secret->len >= hcb->block_size) {
    hcb->init(state);
    hcb->update(state, (const unsigned char*)secret->s, secret->len);
    hcb->finalize(state, block);
    memset(block + hcb->digest_size, 0, sizeof block - hcb->digest_size);
  }
  else {
    memcpy(block, secret->s, secret->len);
    memset(block + secret->len, 0, sizeof block - secret->len);
  }
  for (i = 0; i < sizeof block; ++i)
    block[i] ^= 0x36;

  /* Generate prestate for H(K XOR ipad) */
  hcb->init(state);
  hcb->update(state, block, sizeof block);
  hcb->extract(state, midstate);
  
  /* Generate K XOR opad */
  for (i = 0; i < sizeof block; ++i)
    block[i] ^= 0x36 ^ 0x5c;
  
  /* Generate prestate for H(K XOR opad) */
  hcb->init(state);
  hcb->update(state, block, sizeof block);
  hcb->extract(state, midstate + hcb->midstate_size);

  memset(state, 0, sizeof state);
  memset(block, 0, sizeof block);
}

/** Calculate the final HMAC digest.
 *
 * This function uses the initialization vectors produced by \c
 * hmac_prepare to produce the final digest output from the HMAC.
 */
void hmac_finish(const struct hmac_control_block* hcb,
		 const void* midstate,
		 const str* nonce,
		 void* output)
{
  uint64 statebuf[(hcb->state_size + 7) / sizeof(uint64)];
  unsigned char *state = (unsigned char *)statebuf;

  /* Generate H1 = H(K XOR ipad, nonce) */
  hcb->inject(state, midstate);
  hcb->update(state, (const unsigned char*)nonce->s, nonce->len);
  hcb->finalize(state, output);
  
  /* Generate Output = H(K XOR opad, H1) */
  hcb->inject(state, midstate + hcb->midstate_size);
  hcb->update(state, output, hcb->digest_size);
  hcb->finalize(state, output);

  memset(state, 0, sizeof state);
}

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
  uint64 statebuf[(hcb->state_size * 2 + 7) / sizeof(uint64)];
  unsigned char *midstate = (unsigned char *)statebuf;

  hmac_prepare(hcb, midstate, secret);
  hmac_finish(hcb, midstate, nonce, output);

  memset(midstate, 0, sizeof midstate);
}
