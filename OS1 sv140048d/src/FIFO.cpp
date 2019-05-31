/*
 * FIFO.cpp
 *
 *
 *      Author: Vladimir Sladojevic
 */
#include<stdlib.h>
#include<iostream.h>
#include "FIFO.h"
#include "macros.h"

FIFO::FIFO() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	first=NULL;
	last=NULL;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

void FIFO::put(int* data){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif

	if (first==NULL){
		first=new Elem(data);
		last=first;
	}
	else{
		Elem* temp=new Elem(data);
		last->next=temp;
		last=last->next; //last=temp
	}
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif

}

int* FIFO::get(){

#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	int* retValue=NULL;
	if (last==NULL){// Ako pokusamo da izvadimo iz praznog reda
		retValue=NULL;
	}
	else if(first==last){// Ako postoji samo jedan element u redu
		retValue=first->data;
		delete first;
		first=NULL;
		last=NULL;
	}
	else{
		Elem* toReturn=first;
		retValue=toReturn->data;
		first=first->next;
		delete toReturn;
	}
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif

	return retValue;
}

void FIFO::ispis(){

	Elem* curr=first;
	while(curr!=NULL){
		if (curr->next!=NULL){
			cout<<*curr->data<<"->";
		}
		else cout<<*curr->data<<"->!";
		curr=curr->next;
	}
	cout<<"kraj\n";
}

FIFO::~FIFO() {
	// TODO Auto-generated destructor stub
}

/*
void main(){
	FIFO red=FIFO();
	int a=5;
	int b=4;
	int* pa=&a;
	int* pb=&b;
	red.put(pa);
	red.ispis();
	red.put(pb);
	red.ispis();
	red.get();
	red.ispis();
	red.get();
	red.ispis();
	red.get();
	red.ispis();


}
*/


