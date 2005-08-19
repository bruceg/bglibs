/* $Id$ */
#ifndef UNIX__SIG__H__
#define UNIX__SIG__H__

typedef void (*signalfn)(int);

void sig_block(int sig);
void sig_unblock(int sig);
void sig_suspend(void);
void sig_catch(int sig, signalfn fn);
void sig_default(int sig);
void sig_ignore(int sig);

void sig_misc_ignore(void);
void sig_bug_catch(signalfn fn);

#define SIG_FUNCTION_DECL(NAME) \
extern void sig_##NAME##_block(void); \
extern void sig_##NAME##_unblock(void); \
extern void sig_##NAME##_catch(signalfn fn); \
extern void sig_##NAME##_default(void)

SIG_FUNCTION_DECL(alarm);
SIG_FUNCTION_DECL(child);
SIG_FUNCTION_DECL(hup);
SIG_FUNCTION_DECL(int);
SIG_FUNCTION_DECL(pipe);
SIG_FUNCTION_DECL(quit);
SIG_FUNCTION_DECL(term);

SIG_FUNCTION_DECL(all);

#undef SIG_FUNCTION_DECL

#define SIG_FUNCTION_MAKE(NAME,SIG) \
extern void sig_##NAME##_block(void) { sig_block(SIG); } \
extern void sig_##NAME##_unblock(void) { sig_unblock(SIG); } \
extern void sig_##NAME##_catch(signalfn fn) { sig_catch(SIG, fn); } \
extern void sig_##NAME##_default(void) { sig_default(SIG); } \

#endif
