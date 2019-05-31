#include "PCB.h"
#include "FIFOPCB.h"
#include "Semaphor.h"
#include "Schedule.h"
#include "FIFOSem.h"
#include "Core.h"
#include "IdleT.h"
#include "CoreSem.h"

CoreSem::CoreSem(int init) {
	value = init;
	blocked = new FIFOPCB();
	Core::allSem->put(this);
}

int CoreSem::wait(Time maxTimeToWait) {
	if (--value < 0) {
		PCB::running->state = PCB::BLOCKED;
		PCB::running->semTime = maxTimeToWait; //da li staviti posebnu vrednost za maxTimeToWait 0
		if(maxTimeToWait==0) PCB::running->semInfFlag=1;
		blocked->put(PCB::running);
		dispatch();
	}
	return PCB::running->signalFlag;
}

void CoreSem::signal() {
	if (value++ < 0) {
		PCB* temp = blocked->get();
		temp->state = PCB::READY;
		temp->signalFlag = 1; //postavljamo
		temp->semInfFlag=0;
		//temp->semTime = 0; //restartujemo mu semTime
		if(temp!=Core::idle->myIdlePCB)
		Scheduler::put(temp);
	}
}


/*
void CoreSem::dec(){
	FIFOPCB::Elem* curr=blocked->first;

	while(curr!=NULL){
		if (curr->data->semInfFlag==0)
			curr->data->semTime--;
		curr=curr->next;
	}

	curr=blocked->first;
	FIFOPCB::Elem* prev=NULL;
	while(curr!=NULL){
		if (curr->data->semInfFlag==0 && curr->data->semTime==0){//treba da izbacimo
			FIFOPCB::Elem* old=curr;
			old->data->state=PCB::READY;
			old->data->signalFlag=0;
			if(prev==NULL){//izbacujemo sa pocetka liste
				FIFOPCB::Elem* old=curr;
				curr=curr->next;
				old->next=NULL;
				//deblokiramo
				old->data->state=PCB::READY;
				old->data->signalFlag = 0;
				Scheduler::put(old->data);
				delete old;
			}
			else{
				FIFOPCB::Elem* old=curr;
				prev->next=curr->next;

			}
		}
		else{
			prev=curr;
			curr=curr->next;
		}
	}

}
*/


void CoreSem::dec() { //ne treba lock i unlock ako sam stavio kod Core::decALL
	FIFOPCB::Elem* curr = blocked->first;
	FIFOPCB::Elem* prev = NULL;
	while (curr != NULL) {
		int izbacio = 0; //napravice mnogo promenljivih izbacio
		if (curr->data->semTime != 0) {
			curr->data->semTime--;
			if (curr->data->semTime == 0) {
				//izbaujemo ga iz liste, tj odblokiramo
				izbacio = 1;
				FIFOPCB::Elem* old = curr;
				PCB* temp=curr->data;
				//za deblokiranje
				temp->signalFlag = 0;
				temp->state = PCB::READY;
				//old->data->semTime = 0; //za svaki slucaj, iako bi trebalo da je 0
				if(temp!=Core::idle->myIdlePCB)
				Scheduler::put(temp);

				if (prev == NULL) { //prvi el izbacujemo iz liste
					curr = curr->next;
					//delete old;
				} else {
					prev->next = curr->next;
					curr->next = NULL;
					curr = prev->next;
					//delete old;

				}

			}
		}
		if (izbacio == 0) {
			prev = curr;
			curr = curr->next;
		}
	}
}


CoreSem::~CoreSem() { //treba dodati da izbrise iz Core::allSem
	//delete blocked;
	//mySem=NULL;
	//da li treba pre ili posle

	while (PCB::running->blocked->first) {
				PCB* tempPCB = PCB::running->blocked->remove();
				tempPCB->state = PCB::READY;
				if(tempPCB!=Core::idle->myIdlePCB)
				Scheduler::put(tempPCB);
			}


	/*FIFOSem::Elem* curr=Core::allSem->first;
	 FIFOSem::Elem* prev=NULL;
	 while(curr!=NULL){
	 if (curr->data==this){
	 //izbacujemo
	 if (prev==NULL){
	 Core::allSem->first=curr->next;
	 curr->next=NULL;
	 delete curr;
	 break;
	 }
	 else{
	 prev->next=curr->next;
	 curr->next=NULL;
	 delete curr;
	 break;
	 }
	 }
	 else {prev=curr; curr=curr->next;}
	 }*/
}

int CoreSem::val() const {
	return value;
}

