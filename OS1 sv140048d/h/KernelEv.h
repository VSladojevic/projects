/*
 * KernelEv.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

class IVTEntry;

typedef unsigned char IVTNo;
extern IVTEntry *IVTarray[];

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();
	friend class PCB;
	friend class Event;

	Event* myEvent;

	void block();
	void unblock();
	int value;
	IVTNo ivtNo;
	PCB* master;
};
#endif /* KERNELEV_H_ */
