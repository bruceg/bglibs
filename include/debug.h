#ifndef MSG__DEBUG__H__
#define MSG__DEBUG__H__

#include <stdarg.h>

extern int msg_debug_bits;

#ifdef MSG_NO_DEBUG
#define msg_debug(I,A,B,C,D,E,F,X) do{}while(0)
#define msg_debugf(I,X,F,...) do{}while(0)
#else
void msg_debug(int, const char*, const char*, const char*, const char*, const char*, const char*, int);
void msg_debugf(int bit, int showsys, const char* format, ...);
#endif
void msg_debug_init(void);

#define debug1(x,a)           msg_debug(x,a,0,0,0,0,0,0)
#define debug2(x,a,b)         msg_debug(x,a,b,0,0,0,0,0)
#define debug3(x,a,b,c)       msg_debug(x,a,b,c,0,0,0,0)
#define debug4(x,a,b,c,d)     msg_debug(x,a,b,c,d,0,0,0)
#define debug5(x,a,b,c,d,e)   msg_debug(x,a,b,c,d,e,0,0)
#define debug6(x,a,b,c,d,e,f) msg_debug(x,a,b,c,d,e,f,0)
#define debugf(x,f,...)       msg_debugf(x,0,f,__VA_ARGS__)

#define debug1sys(x,a)           msg_debug(x,a,0,0,0,0,0,1)
#define debug2sys(x,a,b)         msg_debug(x,a,b,0,0,0,0,1)
#define debug3sys(x,a,b,c)       msg_debug(x,a,b,c,0,0,0,1)
#define debug4sys(x,a,b,c,d)     msg_debug(x,a,b,c,d,0,0,1)
#define debug5sys(x,a,b,c,d,e)   msg_debug(x,a,b,c,d,e,0,1)
#define debug6sys(x,a,b,c,d,e,f) msg_debug(x,a,b,c,d,e,f,1)
#define debugfsys(x,f,...)       msg_debugf(x,1,f,__VA_ARGS__)

#endif
