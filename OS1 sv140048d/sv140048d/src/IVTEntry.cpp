/*
 * IVTEntry.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#include "IVTEntry.h"
#include <dos.h>
#include "KernelEv.h"

IVTEntry::IVTEntry(IVTNo number,IntRoutine newInterRoutine):ivtNo(number),myKernelEv(0){
#ifndef BCC_BLOCK_IGNORE
	IVTarray[number]=this;
	oldRout=getvect(number);
	setvect(number,newInterRoutine);
#endif

}

void IVTEntry::signal(){
	if (myKernelEv!=0) myKernelEv->signal();
}

void IVTEntry::callOldR(){
	(*oldRout)();
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo,oldRout);
#endif

}



