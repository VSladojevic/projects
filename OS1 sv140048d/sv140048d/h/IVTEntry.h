/*
 * IVTEntry.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

class KernelEv;

typedef unsigned char IVTNo;
typedef void interrupt (*IntRoutine)(...);

class IVTEntry {
public:
	friend class KernelEv;
	friend class Event;
	IVTEntry(IVTNo iN, IntRoutine newInterRoutine);
	void signal();
	~IVTEntry();
	void callOldR();

	int ivtNo;
	IntRoutine oldRout;
	KernelEv *myKernelEv;
};

#endif /* IVTENTRY_H_ */
