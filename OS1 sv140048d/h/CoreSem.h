/*
 * CoreSem.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef CORESEM_H_
#define CORESEM_H_


class CoreSem {
public:
	CoreSem(int init = 1);
	~CoreSem();
	int wait(unsigned maxTimeToWait);
	void signal();
	int val() const;
	friend class Semaphore;
	Semaphore* mySem;


	void dec();

	friend class FIFOPCB;
	FIFOPCB* blocked;
	int value;
};

#endif /* CORESEM_H_ */
