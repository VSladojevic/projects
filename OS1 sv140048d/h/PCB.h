/*
 * PCB.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"
#include <stdlib.h>

class PCB {
private:

	static ID ukID;



public:
	Thread* myThread;
	PCB(Thread* t=NULL, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual ~PCB();
	void createStack();

	static void setStarting();

	friend class FIFOPCB;
	FIFOPCB* blocked;

	static void wrapper();
	int signalFlag; //semafor
	int semTime; //vreme na semaforu
	int semInfFlag; //da li je Time 0
	// 				  0     1       2      3
	static const int NEW, READY, BLOCKED, OVER;

	static PCB* running;

	ID id;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned* stack;
	StackSize PCBstackSize;
	Time PCBtimeSlice;
	int state; //NEW, READY, BLOCKED, OVER

	int wtcFlag;

};

#endif /* PCB_H_ */
