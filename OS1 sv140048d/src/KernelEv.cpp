/*
 * KernelEv.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#include "PCB.h"
#include "IdleT.h"
#include "Core.h"
#include "Scheduler.h"
#include "KernelEv.h"

typedef unsigned char IVTNo;
KernelEv::KernelEv(IVTNo number){
	value=0;
	ivtNo=number;
	master=PCB::running;
}
IVTEntry *IVTarray[256];

KernelEv::~KernelEv(){
	IVTarray[ivtNo]=0;
}

void KernelEv::wait(){
	if (PCB::running==master){
		if (value==1) value--;
		else if (value==0){
			value--;
			block();
		}
	}
}

void KernelEv::signal(){
	if (value==0) value++;
	else if (value==-1){
		value++;
		unblock();
	}
}

void KernelEv::block(){
	master->state=PCB::BLOCKED;
	dispatch();
}

void KernelEv::unblock(){
	master->state=PCB::READY;
	if (master->state==PCB::READY && master!=Core::idle->myIdlePCB) Scheduler::put(master);
}
