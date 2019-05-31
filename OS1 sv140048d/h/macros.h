/*
 * macros.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef MACROS_H_
#define MACROS_H_

#include "Core.h"

#define LOCK asm { pushf; cli; }
#define UNLOCK asm { popf; }

#define STOPDISP Core::canDispatch=0;
#define FREEDISP {Core::canDispatch=1; if(Core::requested_dispatch) dispatch();}

typedef void interrupt (*InterruptRoutine)(...);

#endif /* MACROS_H_ */

