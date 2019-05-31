/*
 * FIFOSem.cpp
 *
 *  
 *      Author: Vladimir Sladojevic
 */


#include<stdlib.h>
#include<iostream.h>
#include "macros.h"
#include "FIFOSem.h"
//#include "PCB.h"


FIFOSem::FIFOSem() {
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

int FIFOSem::size(){
	return len;
}

void FIFOSem::put(CoreSem* data){
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

CoreSem* FIFOSem::get(){

#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	CoreSem* retValue=NULL;
	if (last==NULL){// Ako pokusamo da izvadimo iz praznog reda
		retValue=NULL;
	}
	else if(first==last){// Ako postoji samo jedan element u redu
		len--;
		retValue=first->data;
		delete first;
		first=NULL;
		last=NULL;
	}
	else{
		len--;
		Elem* toReturn=first;
		retValue=toReturn->data;
		first=first->next;
		delete toReturn;
	}
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif

	return retValue;
}

void FIFOSem::ispis(){

	Elem* curr=first;
	while(curr!=NULL){
		if (curr->next!=NULL){
			cout<<curr->data<<"->";
		}
		else cout<<curr->data<<"->!";
		curr=curr->next;
	}
	cout<<"\n";
}

FIFOSem::~FIFOSem() {
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
