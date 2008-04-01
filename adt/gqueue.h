#ifndef BGLIBS__GENERIC_QUEUE__H__
#define BGLIBS__GENERIC_QUEUE__H__

#include <adt/common.h>

/** \defgroup adt_gqueue adt gqueue: Generic simple queue.
 *
 * A generic queue is a first-in-first-out structure defined here based
 * on three primary operations: push, top, and pop.  Pushing an element
 * onto a queue adds it to the tail of the list.  The top operation
 * fetches the least recently pushed element still on the queue, and
 * popping removes it.
 */

/** @{ */

/** \example adt/gqueue_test.c */

/** Generic queue node. */
struct gqueue_node
{
  /** Pointer to the next element in the queue linked list. */
  struct gqueue_node* next;
  /** The data contained in the queue node. */
  char data[0];
};

/** Generic queue structure. */
struct gqueue
{
  /** Pointer to the first item in the queue. */
  struct gqueue_node* head;
  /** Pointer to the last item in the queue. */
  struct gqueue_node* tail;
  /** The number of items in the queue. */
  unsigned count;
};

int gqueue_push(struct gqueue* d, unsigned datasize, const void* data,
		adt_copy_fn* fn);
void* gqueue_top(const struct gqueue* q);
void gqueue_pop(struct gqueue* q, adt_free_fn* fn);

/** Declare specialized \c gqueue functions. */
#define GQUEUE_DECL(PREFIX,TYPE) \
extern int PREFIX##_push(struct gqueue* q, TYPE const* data); \
extern TYPE* PREFIX##_top(struct gqueue* q); \
extern void PREFIX##_pop(struct gqueue* q);

/** Define a specialized \c gqueue push function. */
#define GQUEUE_PUSH_DEFN(PREFIX,TYPE,COPY) \
int PREFIX##_push(struct gqueue* q, TYPE const* data) { \
  return gqueue_push(q, sizeof *data, data, (adt_copy_fn*)COPY); \
}

/** Define a specialized \c gqueue top function. */
#define GQUEUE_TOP_DEFN(PREFIX,TYPE) \
TYPE* PREFIX##_top(struct gqueue* q) { \
  return (q->head == 0) ? 0 : (TYPE*)q->head->data; \
}

/** Define a specialized \c gqueue pop function. */
#define GQUEUE_POP_DEFN(PREFIX,FREE) \
void PREFIX##_pop(struct gqueue* q) { \
  gqueue_pop(q, (adt_free_fn*)(FREE)); \
}

/** Define all the specialized \c gqueue functions.  If \c COPY is \c
 * NULL, a simple memcpy is used instead.  If \c FREE is \c NULL, no
 * attempt is made to free the data. */
#define GQUEUE_DEFN(PREFIX,TYPE,COPY,FREE) \
GQUEUE_PUSH_DEFN(PREFIX,TYPE,COPY) \
GQUEUE_TOP_DEFN(PREFIX,TYPE) \
GQUEUE_POP_DEFN(PREFIX,FREE)

/** @} */

#endif
