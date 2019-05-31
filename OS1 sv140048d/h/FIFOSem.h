/*
 * FIFOSem.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef FIFOSEM_H_
#define FIFOSEM_H_


#include <stdlib.h>

class CoreSem;


class FIFOSem {
public:
	struct Elem {
		CoreSem* data;
		Elem* next;

		Elem(CoreSem* d){
			data=d;
			next=NULL;
		}

	};
	FIFOSem();
	~FIFOSem();

	int len;
	int size();

	void put(CoreSem* data);
	CoreSem* get();
	void ispis();

	Elem* first;
	Elem* last;
};


#endif /* FIFOSEM_H_ */
