/*
 * IdleT.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#include "Core.h"
#include "Thread.h"
#include "PCB.h"
#include "macros.h"
#include "iostream.h"
#include "IdleT.h"



IdleT::IdleT(): Thread(256, 2) { myIdlePCB=Core::idleTemp; }

void IdleT::run() {
	while (1){/*
#ifndef BCC_BLOCK_IGNORE
		STOPDISP
#endif
		cout<<"U Idle-u"<<endl;
#ifndef BCC_BLOCK_IGNORE
		FREEDISP
#endif
			for (int k = 0; k<10000; ++k)
						for (int j = 0; j <30000; ++j);*/
	}
}

void IdleT::start() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	myIdlePCB->state=PCB::READY;
	myIdlePCB->createStack();
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}
