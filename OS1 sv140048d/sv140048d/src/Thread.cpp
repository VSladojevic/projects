/*
 s * Thread.cpp
 *
 *      Author: Vladimir Sladojevic
 */
#include "Core.h"
#include "Thread.h"
#include "PCB.h"

#include "macros.h"
#include "SCHEDULE.H"
#include <stdlib.h>

#include "FIFOPCB.h"
#include "Core.h"
#include <iostream.h>

void dispatch() {
	Core::dispatch();
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	myPCB = new PCB(this, stackSize, timeSlice);
	if (myPCB->id == 2) {
		Core::idleTemp = myPCB;
	}
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

/*
 void Thread::run() {
 for (int i = 0; i < 5; ++i) {
 #ifndef BCC_BLOCK_IGNORE
 STOPDISP
 #endif
 cout << "U Threadu id: = " << myPCB->id << endl;
 #ifndef BCC_BLOCK_IGNORE
 FREEDISP
 #endif
 for (int k = 0; k < 10000; ++k)
 for (int j = 0; j < 30000; ++j)
 ;
 }
 }
 */

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread * getThreadById(ID id) {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	Thread* res;
	FIFOPCB::Elem * temp = Core::allPCB->first;
	while (temp != NULL) {
		if (temp->data->id == id)
			break;
		temp = temp->next;
	}
	if (temp != NULL)
		res = temp->data->myThread;
	else
		res = NULL;

#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
	return res;
}

void Thread::start() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	myPCB->createStack();
	myPCB->state = PCB::READY;
	Scheduler::put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

void Thread::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	if (myPCB == PCB::running || this == Core::idle
			|| myPCB->state == PCB::OVER) {

#ifndef BCC_BLOCK_IGNORE
		UNLOCK
#endif
	} else {
		PCB::running->state = PCB::BLOCKED;
		myPCB->blocked->put(PCB::running);

#ifndef BCC_BLOCK_IGNORE
		UNLOCK
#endif
		Core::dispatch();
	}
}
/*
void Thread::waitToComplete() {

	STOPDISP
	cout << "Pozvan je wtc za nit " << myPCB->id << ", a running je nit "
			<< PCB::running->id << endl;
	cout<<"Blokirani: ";myPCB->blocked->ispis();
	FREEDISP

	myPCB->wtcFlag = 1;
	if (myPCB->state == PCB::NEW || myPCB->state == PCB::OVER
			|| myPCB == PCB::running || this == Core::starting
			|| this == Core::idle)
		return;

	STOPDISP
	cout << "Blokirace se" << endl;
	FREEDISP

	PCB::running->state = PCB::BLOCKED;
	myPCB->blocked->put(PCB::running);


	STOPDISP
	cout << "Pozvan je wtc za nit " << myPCB->id << ", a running je nit "
			<< PCB::running->id << endl;
	cout<<"Blokirani: ";myPCB->blocked->ispis();
	FREEDISP

	Core::dispatch();

}*/

Thread::~Thread() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif

	//if (myPCB->wtcFlag == 0)
	waitToComplete();

	delete myPCB;

	//myPCB = NULL;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif

}

