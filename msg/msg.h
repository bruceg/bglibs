#ifndef MSG__H__
#define MSG__H__

extern const char program[];
extern const int msg_show_pid;
extern int msg_debug_bits;

void msg_common(const char* type, const char*, const char*, const char*, const char*, const char*, const char*, int);
#define msg(a,b,c,d,e,f) msg_common(0,a,b,c,d,e,f,0)
#ifdef MSG_NO_DEBUG
#define msg_debug(I,A,B,C,D,E,F,X) do{}while(0)
#else
void msg_debug(int, const char*, const char*, const char*, const char*, const char*, const char*, int);
#endif
void msg_warn(const char*, const char*, const char*, const char*, const char*, const char*, int);
void msg_error(const char*, const char*, const char*, const char*, const char*, const char*, int);
void msg_die(int, const char*, const char*, const char*, const char*, const char*, const char*, int);
void msg_debug_init(void);

#define msg1(a)           msg(a,0,0,0,0,0)
#define msg2(a,b)         msg(a,b,0,0,0,0)
#define msg3(a,b,c)       msg(a,b,c,0,0,0)
#define msg4(a,b,c,d)     msg(a,b,c,d,0,0)
#define msg5(a,b,c,d,e)   msg(a,b,c,d,e,0)
#define msg6(a,b,c,d,e,f) msg(a,b,c,d,e,f)

#define debug1(x,a)           msg_debug(x,a,0,0,0,0,0,0)
#define debug2(x,a,b)         msg_debug(x,a,b,0,0,0,0,0)
#define debug3(x,a,b,c)       msg_debug(x,a,b,c,0,0,0,0)
#define debug4(x,a,b,c,d)     msg_debug(x,a,b,c,d,0,0,0)
#define debug5(x,a,b,c,d,e)   msg_debug(x,a,b,c,d,e,0,0)
#define debug6(x,a,b,c,d,e,f) msg_debug(x,a,b,c,d,e,f,0)

#define warn1(a)           msg_warn(a,0,0,0,0,0,0)
#define warn2(a,b)         msg_warn(a,b,0,0,0,0,0)
#define warn3(a,b,c)       msg_warn(a,b,c,0,0,0,0)
#define warn4(a,b,c,d)     msg_warn(a,b,c,d,0,0,0)
#define warn5(a,b,c,d,e)   msg_warn(a,b,c,d,e,0,0)
#define warn6(a,b,c,d,e,f) msg_warn(a,b,c,d,e,f,0)

#define error1(a)           msg_error(a,0,0,0,0,0,0)
#define error2(a,b)         msg_error(a,b,0,0,0,0,0)
#define error3(a,b,c)       msg_error(a,b,c,0,0,0,0)
#define error4(a,b,c,d)     msg_error(a,b,c,d,0,0,0)
#define error5(a,b,c,d,e)   msg_error(a,b,c,d,e,0,0)
#define error6(a,b,c,d,e,f) msg_error(a,b,c,d,e,f,0)

#define die1(x,a)           msg_die(x,a,0,0,0,0,0,0)
#define die2(x,a,b)         msg_die(x,a,b,0,0,0,0,0)
#define die3(x,a,b,c)       msg_die(x,a,b,c,0,0,0,0)
#define die4(x,a,b,c,d)     msg_die(x,a,b,c,d,0,0,0)
#define die5(x,a,b,c,d,e)   msg_die(x,a,b,c,d,e,0,0)
#define die6(x,a,b,c,d,e,f) msg_die(x,a,b,c,d,e,f,0)

#define debug1sys(x,a)           msg_debug(x,a,0,0,0,0,0,1)
#define debug2sys(x,a,b)         msg_debug(x,a,b,0,0,0,0,1)
#define debug3sys(x,a,b,c)       msg_debug(x,a,b,c,0,0,0,1)
#define debug4sys(x,a,b,c,d)     msg_debug(x,a,b,c,d,0,0,1)
#define debug5sys(x,a,b,c,d,e)   msg_debug(x,a,b,c,d,e,0,1)
#define debug6sys(x,a,b,c,d,e,f) msg_debug(x,a,b,c,d,e,f,1)

#define warn1sys(a)           msg_warn(a,0,0,0,0,0,1)
#define warn2sys(a,b)         msg_warn(a,b,0,0,0,0,1)
#define warn3sys(a,b,c)       msg_warn(a,b,c,0,0,0,1)
#define warn4sys(a,b,c,d)     msg_warn(a,b,c,d,0,0,1)
#define warn5sys(a,b,c,d,e)   msg_warn(a,b,c,d,e,0,1)
#define warn6sys(a,b,c,d,e,f) msg_warn(a,b,c,d,e,f,1)

#define error1sys(a)           msg_error(a,0,0,0,0,0,1)
#define error2sys(a,b)         msg_error(a,b,0,0,0,0,1)
#define error3sys(a,b,c)       msg_error(a,b,c,0,0,0,1)
#define error4sys(a,b,c,d)     msg_error(a,b,c,d,0,0,1)
#define error5sys(a,b,c,d,e)   msg_error(a,b,c,d,e,0,1)
#define error6sys(a,b,c,d,e,f) msg_error(a,b,c,d,e,f,1)

#define die1sys(x,a)           msg_die(x,a,0,0,0,0,0,1)
#define die2sys(x,a,b)         msg_die(x,a,b,0,0,0,0,1)
#define die3sys(x,a,b,c)       msg_die(x,a,b,c,0,0,0,1)
#define die4sys(x,a,b,c,d)     msg_die(x,a,b,c,d,0,0,1)
#define die5sys(x,a,b,c,d,e)   msg_die(x,a,b,c,d,e,0,1)
#define die6sys(x,a,b,c,d,e,f) msg_die(x,a,b,c,d,e,f,1)

extern void die_oom(int);

#endif
