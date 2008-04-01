#ifndef BGLIBS__MISC__UCSPI__H__
#define BGLIBS__MISC__UCSPI__H__

extern const char* ucspi_protocol(void);
extern const char* ucspi_getenv(const char* name);

#define ucspi_localip()    ucspi_getenv("LOCALIP")
#define ucspi_localhost()  ucspi_getenv("LOCALHOST")
#define ucspi_remoteip()   ucspi_getenv("REMOTEIP")
#define ucspi_remotehost() ucspi_getenv("REMOTEHOST")

#endif
