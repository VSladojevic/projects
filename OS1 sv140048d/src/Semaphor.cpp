/*
 * Semaphor.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */


#include "macros.h"
#include "CoreSem.h"
#include "Semaphor.h"


Semaphore::Semaphore(int init){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	myImpl = new CoreSem(init);
	myImpl->mySem=this;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

int Semaphore::val() const{
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	int i = myImpl->val();
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
	return i;
}

Semaphore::~Semaphore(){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

int Semaphore::wait(Time time){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	int value = myImpl->wait(time);
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
	return value;
}

void Semaphore::signal(){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}
