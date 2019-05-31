/*
 * Core.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef CORE_H_
#define CORE_H_

//#include "IdleT.h" // problem?

//Globalne funkcije i promenljive

void interrupt timer(...);
void inicTimer();
void restoreTimer();

extern void tick();

class Core {
public:
	friend class PCB;
	friend class Thread; //umesto include IdleT
	friend class IdleT; //umesto include IdleT


	friend class FIFOPCB;
	friend class FIFOSem;

	static PCB* idleTemp;
	static FIFOSem* allSem;
	static FIFOPCB* allPCB;
	static void dispatch();
	static void decAllSem();

	static int canDispatch;
	static int i;
	static int requested_dispatch;
	static int brojac;
	static Thread* starting;
	static IdleT* idle;

};

#endif /* CORE_H_ */
