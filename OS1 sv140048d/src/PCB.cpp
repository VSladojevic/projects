/*
 * PCB.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#include "Thread.h"
#include "PCB.h"
#include <dos.h>
#include <iostream.h>
#include <macros.h>
#include <stdlib.h>

#include "Scheduler.h"
#include "IdleT.h"
#include "FIFOPCB.h"
#include "Core.h"

ID PCB::ukID = 0;

PCB* PCB::running = NULL;

const int PCB::NEW = 0;
const int PCB::READY = 1;
const int PCB::BLOCKED = 2;
const int PCB::OVER = 3;


PCB::PCB(Thread * t, StackSize stackSize, Time timeSlice) {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif

	wtcFlag=0;

	blocked=new FIFOPCB();
	signalFlag = 0;
	semTime=0; //da li 0
	semInfFlag=0;
	myThread = t;
	id = ++ukID;

	if (stackSize > 65536)
		stackSize = 65536;
	stackSize /= sizeof(unsigned);
	PCBstackSize = stackSize;

	PCBtimeSlice = timeSlice;
	state = NEW;

	Core::allPCB->put(this);

#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}


void PCB::setStarting(){
	Core::starting=new Thread();
	Core::starting->myPCB->state=PCB::READY;
	PCB::running=Core::starting->myPCB;
}

void PCB::createStack() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	stack = new unsigned[PCBstackSize];
	stack[PCBstackSize - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[PCBstackSize-2]=FP_SEG(&(PCB::wrapper));
	stack[PCBstackSize-3]=FP_OFF(&(PCB::wrapper));
	sp = FP_OFF(stack + PCBstackSize - 12);
	ss = FP_SEG(stack + PCBstackSize - 12);
#endif
	bp = sp;

#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}


PCB::~PCB() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	delete blocked;
	myThread=NULL;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

void PCB::wrapper() {

	//cout<<"AJDEEEEE";
	running->myThread->run();
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	running->state = PCB::OVER;

	// treba da se oslobode blokirane
	// zatim UNLOCK

	while (PCB::running->blocked->first) {
			PCB* tempPCB = PCB::running->blocked->remove();
			tempPCB->state = READY;
			if (tempPCB!=Core::idle->myIdlePCB)
			Scheduler::put(tempPCB);
		}

/*

	PCB* temp;
	while( running->blocked->len > 0 )
	{

		temp = running->blocked->get();
		temp->state = PCB::READY;
		Scheduler::put(temp);
	}
*/

#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif

	Core::dispatch();

}
