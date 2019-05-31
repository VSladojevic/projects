/*
 * Event.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#include "KernelEv.h"
#include "IVTEntry.h"
#include "macros.h"
#include "Event.h"

Event::Event(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	myImpl=new KernelEv(ivtNo);
	myImpl->myEvent=this;
	IVTarray[ivtNo]->myKernelEv=myImpl;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

Event::~Event(){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

void Event::wait(){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

void Event::signal(){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}
