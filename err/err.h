#ifndef ERR__H__
#define ERR__H__

extern const char program[];

void err_message(const char* type, const char*, const char*, const char*, const char*, int);
void warn(const char*, const char*, const char*, const char*, int);
void error(const char*, const char*, const char*, const char*, int);
void die(int, const char*, const char*, const char*, const char*, int);

#define warn1(a) warn(a,0,0,0,0)
#define warn2(a,b) warn(a,b,0,0,0)
#define warn3(a,b,c) warn(a,b,c,0,0)
#define warn4(a,b,c,d) warn(a,b,c,d,0)

#define error1(a) error(a,0,0,0,0)
#define error2(a,b) error(a,b,0,0,0)
#define error3(a,b,c) error(a,b,c,0,0)
#define error4(a,b,c,d) error(a,b,c,d,0)

#define die1(x,a) die(x,a,0,0,0,0)
#define die2(x,a,b) die(x,a,b,0,0,0)
#define die3(x,a,b,c) die(x,a,b,c,0,0)
#define die4(x,a,b,c,d) die(x,a,b,c,d,0)

#define warn1sys(a) warn(a,0,0,0,1)
#define warn2sys(a,b) warn(a,b,0,0,1)
#define warn3sys(a,b,c) warn(a,b,c,0,1)
#define warn4sys(a,b,c,d) warn(a,b,c,d,1)

#define error1sys(a) error(a,0,0,0,1)
#define error2sys(a,b) error(a,b,0,0,1)
#define error3sys(a,b,c) error(a,b,c,0,1)
#define error4sys(a,b,c,d) error(a,b,c,d,1)

#define die1sys(x,a) die(x,a,0,0,0,1)
#define die2sys(x,a,b) die(x,a,b,0,0,1)
#define die3sys(x,a,b,c) die(x,a,b,c,0,1)
#define die4sys(x,a,b,c,d) die(x,a,b,c,d,1)

#endif
