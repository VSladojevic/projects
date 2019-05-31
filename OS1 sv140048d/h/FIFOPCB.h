/*
 * FIFOPCB.h
 *
 *  
 *      Author: Vladimir Sladojevic
 */

#ifndef FIFOPCB_H_
#define FIFOPCB_H_

#include <stdlib.h>

class PCB;




class FIFOPCB {
public:
	struct Elem {
		PCB* data;
		Elem* next;

		Elem(PCB* d){
			data=d;
			next=NULL;
		}

	};
	FIFOPCB();
	~FIFOPCB();

	PCB* remove();

	int len;
	int size();

	void put(PCB* data);
	PCB* get();
	void ispis();

	Elem* first;
	Elem* last;
};

#endif /* FIFOPCB_H_ */
