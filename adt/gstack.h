/* $Id$ */
#ifndef BGLIBS__GENERIC_STACK__H__
#define BGLIBS__GENERIC_STACK__H__

#include <adt/common.h>

/** \defgroup adt_gstack adt gstack: Generic simple stack.
 *
 * A generic stack is a first-in-last-out structure defined here based
 * on three primary operations: push, top, and pop.  Pushing an element
 * onto a stack adds it to the head of the list.  The top operation
 * fetches the most recently pushed element still on the stack, and
 * popping removes it.
 */

/** @{ */

/** \example adt/gstack_test.c */

/** Generic stack node. */
struct gstack_node
{
  /** Pointer to the next element in the stack linked list. */
  struct gstack_node* next;
  /** The data contained in the stack node. */
  char data[0];
};

/** Generic stack structure. */
struct gstack
{
  /** Pointer to the first item in the stack. */
  struct gstack_node* head;
  /** The number of items in the stack. */
  unsigned count;
};

int gstack_push(struct gstack* d, unsigned datasize, const void* data,
		adt_copy_fn* fn);
void* gstack_top(const struct gstack* s);
void gstack_pop(struct gstack* s, adt_free_fn* fn);

/** Declare specialized \c gstack functions. */
#define GSTACK_DECL(PREFIX,TYPE) \
extern int PREFIX##_push(struct gstack* s, TYPE const* data); \
extern TYPE* PREFIX##_top(struct gstack* s); \
extern void PREFIX##_pop(struct gstack* s);

/** Define a specialized \c gstack push function. */
#define GSTACK_PUSH_DEFN(PREFIX,TYPE,COPY) \
int PREFIX##_push(struct gstack* s, TYPE const* data) { \
  return gstack_push(s, sizeof *data, data, (adt_copy_fn*)COPY); \
}

/** Define a specialized \c gstack top function. */
#define GSTACK_TOP_DEFN(PREFIX,TYPE) \
TYPE* PREFIX##_top(struct gstack* s) { \
  return (s->head == 0) ? 0 : (TYPE*)s->head->data; \
}

/** Define a specialized \c gstack pop function. */
#define GSTACK_POP_DEFN(PREFIX,FREE) \
void PREFIX##_pop(struct gstack* s) { \
  gstack_pop(s, (adt_free_fn*)(FREE)); \
}

/** Define all the specialized \c gstack functions.  If \c COPY is \c
 * NULL, a simple memcpy is used instead.  If \c FREE is \c NULL, no
 * attempt is made to free the data. */
#define GSTACK_DEFN(PREFIX,TYPE,COPY,FREE) \
GSTACK_PUSH_DEFN(PREFIX,TYPE,COPY) \
GSTACK_TOP_DEFN(PREFIX,TYPE) \
GSTACK_POP_DEFN(PREFIX,FREE)

/** @} */

#endif
