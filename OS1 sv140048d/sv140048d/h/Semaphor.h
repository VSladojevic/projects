/*
 * Semaphor.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_


typedef unsigned int Time;

class CoreSem;
class Semaphore {
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();
	virtual int wait(Time maxTimeToWait);
	virtual void signal();
	int val() const; // Returns the current value of the semaphore
private:
	CoreSem* myImpl;
};

#endif /* SEMAPHOR_H_ */
