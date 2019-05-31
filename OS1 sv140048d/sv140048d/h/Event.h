/*
 * Event.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "IVTEntry.h"

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry,&inter##numEntry);\
void interrupt inter##numEntry(...){\
newEntry##numEntry.signal();\
if (callOld == 1)\
newEntry##numEntry.callOldR();\
dispatch();\
}


typedef unsigned char IVTNo;
class KernelEv;

class Event{
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv *myImpl;
};

#endif /* EVENT_H_ */
