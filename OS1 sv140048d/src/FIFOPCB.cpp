/*
 * FIFOPCB.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */


#include<stdlib.h>
#include<iostream.h>
#include "macros.h"
#include "FIFOPCB.h"
#include "PCB.h"


FIFOPCB::FIFOPCB() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	len=0;
	first=NULL;
	last=NULL;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

PCB* FIFOPCB::remove() {
	Elem* temp = first;
	PCB* tempPCB = temp->data;
	if (temp == NULL)
		return NULL;
	if (temp->next == NULL)
		last = first = NULL;
	else
		first = first->next;
	delete temp;
	len--;
	return tempPCB;
}

int FIFOPCB::size(){
	return len;
}

void FIFOPCB::put(PCB* data){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	len++;
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

PCB* FIFOPCB::get(){

#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	if (--len<0) len=0;
	PCB* retValue=NULL;
	if (last==NULL){// Ako pokusamo da izvadimo iz praznog reda
		retValue=NULL;
	}
	else if(first==last){// Ako postoji samo jedan element u redu
		//len--;
		retValue=first->data;
		//delete first;
		first=NULL;
		last=NULL;
	}
	else{
		//len--;
		Elem* toReturn=first;
		retValue=toReturn->data;
		first=first->next;
		//delete toReturn;
	}
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif

	return retValue;
}

void FIFOPCB::ispis(){

	Elem* curr=first;
	while(curr!=NULL){
		if (curr->next!=NULL){
			cout<<curr->data->id<<"->";
		}
		else cout<<curr->data->id<<"->!";
		curr=curr->next;
	}
	cout<<"\n";
}

FIFOPCB::~FIFOPCB() {
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	Elem* curr=first;
	Elem* stari;
	while(curr!=NULL){
		stari=curr;
		curr=curr->next;
		delete stari;
	}
	len=0;
	first=NULL;
	last=NULL;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}
