/*
 * main.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#include "IdleT.h"
#include "Core.h"
#include "Thread.h"
#include "PCB.h"

#include "SCHEDULE.H"
#include "macros.h"
#include "FIFOPCB.h"

#include <iostream.h>
#include <stdlib.h>

/*
 int main(){

 FIFOPCB* niz=new FIFOPCB();

 return 0;
 }
 */

extern int userMain(int argc, char *argv[]);

int main(int argc, char* argv[]) {

	//PCB* temp=new PCB(NULL);
	//PCB::running = temp;
	inicTimer();

	PCB::setStarting();



	Core::idle = new IdleT();

	Core::idle->start();

	int rezultat = userMain(argc, argv);

	delete Core::idle;
	delete Core::starting; //zasto prvo starting pa idle, hoce li se uopste obrisati starting i idle

	restoreTimer();

	return rezultat;
}

