/* $Id$ */
#ifndef MISC__H__
#define MISC__H__

#include <sysdeps.h>
extern void random_init(uint32 seed);
extern uint32 random_int(void);
#define random_float() (random_int() * (double)(1.0/4294967296.0))
#define random_scale(S) ((unsigned int)(random_float() * (S)))
#define random_trunc(T) (random_int() % (T))

unsigned long strtou(const char* str, const char** end);
const char* utoa(unsigned long);
char* utoa2(unsigned long, char*);

#endif
