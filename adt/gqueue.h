#ifndef BGLIBS__GENERIC_QUEUE__H__
#define BGLIBS__GENERIC_QUEUE__H__

#include <adt/common.h>

struct gqueue_node
{
  struct gqueue_node* next;
  char data[0];
};

struct gqueue
{
  struct gqueue_node* head;
  struct gqueue_node* tail;
  unsigned count;
};

int gqueue_push(struct gqueue* d, unsigned datasize, const void* data,
		adt_copy_fn* fn);
void* gqueue_top(const struct gqueue* q);
void gqueue_pop(struct gqueue* q, adt_free_fn* fn);

#define GQUEUE_DECL(PREFIX,TYPE) \
extern int PREFIX##_push(struct gqueue* q, TYPE const* data); \
extern TYPE* PREFIX##_top(struct gqueue* q); \
extern void PREFIX##_pop(struct gqueue* q);

#define GQUEUE_PUSH_DEFN(PREFIX,TYPE,COPY) \
int PREFIX##_push(struct gqueue* q, TYPE const* data) { \
  return gqueue_push(s, sizeof *data, data, (adt_copy_fn*)COPY); \
}

#define GQUEUE_TOP_DEFN(PREFIX,TYPE) \
TYPE* PREFIX##_top(struct gqueue* q) { \
  return (s->head == 0) ? 0 : (TYPE*)s->head->data; \
}

#define GQUEUE_POP_DEFN(PREFIX,FREE) \
void PREFIX##_pop(struct gqueue* q) { \
  gqueue_pop(s, (adt_free_fn*)(FREE)); \
}

#define GQUEUE_DEFN(PREFIX,TYPE,COPY,FREE) \
GQUEUE_PUSH_DEFN(PREFIX,TYPE,COPY) \
GQUEUE_TOP_DEFN(PREFIX,TYPE) \
GQUEUE_POP_DEFN(PREFIX,FREE)

#endif
