/*
 * IdleT.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef IDLET_H_
#define IDLET_H_

#include "Thread.h" // problem?

class IdleT: public Thread {
public:
	PCB* myIdlePCB;
	IdleT();
	virtual void run();
	void start();
};

#endif /* IDLET_H_ */
