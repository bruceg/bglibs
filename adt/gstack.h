/* $Id$ */
#ifndef BGLIBS__GENERIC_STACK__H__
#define BGLIBS__GENERIC_STACK__H__

#include <adt/common.h>

struct gstack_node
{
  struct gstack_node* next;
  char data[0];
};

struct gstack
{
  struct gstack_node* head;
  unsigned count;
};

int gstack_push(struct gstack* d, unsigned datasize, const void* data,
		adt_copy_fn* fn);
void* gstack_top(const struct gstack* s);
void gstack_pop(struct gstack* s, adt_free_fn* fn);

#define GSTACK_DECL(PREFIX,TYPE) \
extern int PREFIX##_push(struct gstack* s, TYPE const* data); \
extern TYPE* PREFIX##_top(struct gstack* s); \
extern void PREFIX##_pop(struct gstack* s);

#define GSTACK_PUSH_DEFN(PREFIX,TYPE,COPY) \
int PREFIX##_push(struct gstack* s, TYPE const* data) { \
  return gstack_push(s, sizeof *data, data, (adt_copy_fn*)COPY); \
}

#define GSTACK_TOP_DEFN(PREFIX,TYPE) \
TYPE* PREFIX##_top(struct gstack* s) { \
  return (s->head == 0) ? 0 : (TYPE*)s->head->data; \
}

#define GSTACK_POP_DEFN(PREFIX,FREE) \
void PREFIX##_pop(struct gstack* s) { \
  gstack_pop(s, (adt_free_fn*)(FREE)); \
}

#define GSTACK_DEFN(PREFIX,TYPE,COPY,FREE) \
GSTACK_PUSH_DEFN(PREFIX,TYPE,COPY) \
GSTACK_TOP_DEFN(PREFIX,TYPE) \
GSTACK_POP_DEFN(PREFIX,FREE)

#endif
