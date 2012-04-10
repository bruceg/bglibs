#ifndef BGLIBS__UNIX__TRIGGER__H__
#define BGLIBS__UNIX__TRIGGER__H__

#include "sysdeps.h"

/** \defgroup trigger trigger: Safe external event notification

The \c trigger functions provide a safe mechanism for one program to
notify another that an event has happened, using a named pipe.

The traditional UNIX mechanism for doing this has been to use signals.
Signals, however, suffer from several problems.  First, in order to
deliver a signal, the sender needs to determine the PID of the
recipient.  The process of determining the PID is filled with race
conditions leaving the possibility of delivering the signal to the wrong
process.  On the receiving side, receiving the signal puts the receiver
into an execution context where the set of safe system calls is
restricted.

By using a named pipe, the trigger mechanism avoids both these problems.
First, there are no races in accessing the trigger, and no blocking
paths either.  Secondly, the receiver waits for an event by polling the
file descriptor.  Upon receiving an event, the receiver is in the same
state as normal.

Such events might include a new message being added to a queue etc.  No
data is delivered with the event.

@{ */

extern int trigger_set(iopoll_fd* io, const char* path);

/** Determine if a trigger has been pulled, after a polling loop
    indicates activity. */
#define trigger_pulled(IO) ((IO)->revents)

extern void trigger_pull(const char* path);

/** @} */

#endif
