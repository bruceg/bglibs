/* $Id$ */

#include <string.h>
#include <uint64.h>
#include "hash.h"

struct hash_context* hash_alloc(const struct hash_control_block* hcb)
{
  unsigned long size;
  struct hash_context* hc;
  size = sizeof(struct hash_context)
    + hcb->state_size
    + (1 << hcb->block_shift);
  if ((hc = malloc(size)) != 0) {
    hc->hcb = hcb;
    hash_init(hc);
  }
  return hc;
}

void hash_init(struct hash_context* hc)
{
  const struct hash_control_block* hcb = hc->hcb;
  hc->bytes = 0;
  memcpy(hc->data, hcb->init, hcb->state_size);
}

void hash_update(struct hash_context* hc,
		 const unsigned char* data, unsigned long len)
{
  const struct hash_control_block* hcb = hc->hcb;
  unsigned char* buffer = hc->data + hcb->state_size;
  const unsigned long mask = hcb->block_mask;
  const unsigned block_size = 1 << hcb->block_shift;
  unsigned long used = hc->bytes & mask;

  hc->bytes += len;

  /* If there was data left in the buffer, add to it.  If the result is
   * a full block, run it through the compress function. */
  if (used > 0) {
    unsigned long add = block_size - used;
    if (add > len)
      add = len;
    memcpy(buffer+used, data, add);
    len -= add;
    data += add;
    used += add;
    if (used == block_size)
      hcb->compress((struct hash_state*)hc->data, buffer);
  }
  /* Run any full input blocks through the compress function. */
  while (len >= block_size) {
    hcb->compress((struct hash_state*)hc->data, data);
    len -= block_size;
    data += block_size;
  }
  /* At the end, either the buffer is empty or the input is empty, so
   * copy what remains of the input into the start of the buffer. */
  memcpy(buffer, data, len);
}

void hash_finish(struct hash_context* hc, unsigned char* digest)
{
  const struct hash_control_block* hcb = hc->hcb;
  unsigned char* buffer = hc->data + hcb->state_size;
  const unsigned long mask = hcb->block_mask;
  const unsigned block_size = 1 << hcb->block_shift;
  unsigned long used = hc->bytes & mask;
  unsigned long left;
  const unsigned long length = hcb->length_size;

  buffer[used++] = 0x80;
  left = block_size - used;
  if (left < length) {
    memset(buffer+used, 0, left);
    hcb->compress((struct hash_state*)hc->data, buffer);
    used = 0;
    left = block_size;
  }

  memset(buffer+used, 0, left - length);
  hcb->packlength(hc->bytes, buffer + block_size - length);
  hcb->compress((struct hash_state*)hc->data, buffer);

  hcb->extract((struct hash_state*)hc->data, digest);

  memset((struct hash_state*)hc->data, 0, hcb->state_size);
}

int hash_block(const struct hash_control_block* hcb,
	       const unsigned char* data, unsigned long len,
	       unsigned char* digest)
{
  struct hash_context* hc;
  if ((hc = hash_alloc(hcb)) == 0)
    return 0;
  hash_update(hc, data, len);
  hash_finish(hc, digest);
  free(hc);
  return 1;
}
