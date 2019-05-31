/*
 * Core.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#include <dos.h>
#include "macros.h"
#include <stdlib.h>
#include <iostream.h>
#include "IdleT.h"
#include "Thread.h"
#include "FIFOPCB.h"
#include "FIFOSem.h"
#include "PCB.h"
#include "CoreSem.h"

#include "Core.h"

#include "SCHEDULE.H"
FIFOPCB* Core::allPCB = new FIFOPCB();
FIFOSem* Core::allSem = new FIFOSem();
int Core::i = 0;


PCB* Core::idleTemp = NULL;
Thread* Core::starting = NULL;
IdleT* Core::idle = NULL;

unsigned tsp = 0;
unsigned tss = 0;
unsigned tbp = 0;
int Core::requested_dispatch = 0;
int Core::brojac = 3;
int Core::canDispatch = 1;

//void tick(){}//brisem kad dodam test primer

extern void tick();

void interrupt timer(...) {

	//cout<<PCB::running<< " " << Core::brojac << endl;

	if (!Core::requested_dispatch) {
		if (PCB::running->PCBtimeSlice != 0)
			Core::brojac--;
		Core::decAllSem();
		asm int 60h; tick();
	}
	if ((PCB::running->PCBtimeSlice != 0 && Core::brojac <= 0)
			|| Core::requested_dispatch) {

		if (Core::canDispatch) {
			Core::requested_dispatch = 0;

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			//cout<<"OLD: "<<PCB::running->id;
			if (PCB::running->state == PCB::READY
					&& PCB::running != Core::idle->myIdlePCB) {
				Scheduler::put(PCB::running);
				//if (PCB::running==Core::starting->myPCB) cout<<"Ubacio starting"<<endl;
			}

			PCB::running = Scheduler::get();
			if (PCB::running == NULL)
				PCB::running = Core::idle->myIdlePCB;

			//if (PCB::running==Core::starting->myPCB) cout<<"Izvadio starting"<<endl;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			Core::brojac = PCB::running->PCBtimeSlice;
			//cout<<"OLD"<<PCB::running->id;
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif
		}

		else { //od canDispatch-a
			Core::requested_dispatch = 1;
		}
	}

}

void Core::decAllSem() { //da li treba LOCK i UNLOCK
#ifndef BCC_BLOCK_IGNORE
LOCK
#endif
	FIFOSem::Elem* curr = Core::allSem->first;
	while (curr != NULL) {
		curr->data->dec();
		curr = curr->next;
	}
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

void Core::dispatch() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	Core::requested_dispatch = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

InterruptRoutine oldTimer = NULL;
InterruptRoutine newTimer = timer;

void inicTimer() { // postavlja novu prekidnu rutinu
#ifndef BCC_BLOCK_IGNORE
LOCK
oldTimer = getvect(0x08);
setvect(0x08, newTimer);
setvect(0x60, oldTimer);
UNLOCK
#endif


}

void restoreTimer() { // restore-uje prekidne rutine u prvobitno stanje
#ifndef BCC_BLOCK_IGNORE
LOCK
oldTimer = getvect(0x60);
setvect(0x08, oldTimer);
UNLOCK
#endif
}

