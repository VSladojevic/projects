#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "buddy.h"


using namespace std;


void* Buddy::startOfBlock(const void * mem)
{
	unsigned ind;
	int k = (char*)mem - (char*)startAdr;
	ind= k / BLOCK_SIZE;
	int size = 0;
	while (allBlocks[ind] == -2) {
		unsigned buddy = myBuddy(ind, size);
		ind = ind < buddy ? ind : buddy;
		size++;
		//ind--;
	}
	if (allBlocks[ind] == -1) return ind * BLOCK_SIZE + (char *)startAdr;
	return nullptr;
}


int Buddy::block_size_on_level(int lvl){ // pocevsi od nultog nivoa
	return pow(2, NUM_LEVELS - lvl);
}

int Buddy::level_from_block_size(int size){
	return NUM_LEVELS - log2(size);
}

Buddy::Buddy(void* mem, int NO_BLOCKS, int mainCache_size){
	_my_mem = mem;
	_my_mem = (char*)mem + sizeof(Buddy);
	NUM_BLOCKS = NO_BLOCKS;
	NUM_LEVELS = log2(NUM_BLOCKS);
	allBlocks = new (_my_mem) short[NUM_BLOCKS];
	_my_mem = (char*)_my_mem + sizeof(allBlocks);
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		allBlocks[i] = -2;
	}
	
	for (int i = 0; i <= NUM_LEVELS; i++)
	{
		freeBlocks[i] = new ((char*)_my_mem+i*sizeof(chunk))chunk;
		freeBlocks[i]->next = nullptr;
		freeBlocks[i]->prev = nullptr;
		freeBlocks[i] = nullptr;
	}
	_my_mem = (char*)_my_mem + sizeof(freeBlocks);

	//int sysSpace = NUM_BLOCKS*sizeof(short) + MAX_LEVELS*sizeof(chunk) + mainCache_size;

	sysBlocks = ((char*)_my_mem - mem + mainCache_size) / (sizeof(char)*BLOCK_SIZE) + 1;
	int memoryBlocks = NUM_BLOCKS - sysBlocks;
	startAdr = (void*)((char*)mem + BLOCK_SIZE*sysBlocks); // prvi objekat

	int _zeros = NUM_LEVELS;
	start_index = sysBlocks; // bitno!!!
	int MASK = 1 << _zeros;
	while (_zeros >= 0){
		if (MASK & memoryBlocks){
			// komad velicine MASK je slobodan, treba ga dodati u freeBlocks 
			// na odgovarajuci nivo, a allBlocks ne treba menjati jer je vec 0
			int lvl = NUM_LEVELS - _zeros;

			allBlocks[start_index] = NUM_LEVELS - lvl; // bitno!!!
			start_index += block_size_on_level(lvl);
			//if (freeBlocks[lvl] != nullptr){
			

			chunk* _new = new(startAdr)chunk;
			_new->prev = nullptr;
			_new->next = nullptr;
			startAdr = (char*)startAdr + BLOCK_SIZE*block_size_on_level(lvl);


			//_new->next = freeBlocks[lvl];
			//freeBlocks[lvl]->prev = _new;
			freeBlocks[lvl] = _new; // jer sigurno je bilo prazno i samo cemo jedan element dodati
			//}
			
		}

		MASK >>= 1;
		_zeros--;

	}


	startAdr = (void*)((char*)mem + BLOCK_SIZE*sysBlocks);
	start_index = sysBlocks;

}




void* Buddy::takeMem(int size_blocks){
	// size_blocks je stepen dvojke
	mtx.lock();
	int max_size_blocks = block_size_on_level(0);
	if (size_blocks > max_size_blocks){
		mtx.unlock();
		return nullptr;
	}
	int lvl = level_from_block_size(size_blocks);
	while (freeBlocks[lvl] == nullptr){
		lvl--;
		if (lvl < 0){
			mtx.unlock();
			return nullptr; // nema slobodnih blokova!!!
		}
	}
	while (block_size_on_level(lvl) > size_blocks){
		// radimo split
		// prvo izbacimo iz liste na nivou lvl
		chunk* _toSplit = freeBlocks[lvl];
		if (freeBlocks[lvl]->next)
			freeBlocks[lvl]->next->prev = nullptr;
		freeBlocks[lvl] = freeBlocks[lvl]->next;
		_toSplit->next = nullptr;

		// pa dodamo nova dva u listu na nivou lvl+1

		chunk* _first = _toSplit;
		lvl += 1;
		chunk* _second = (chunk*)((char*)_toSplit + BLOCK_SIZE*block_size_on_level(lvl));


		allBlocks[((char*)_first - (char*)startAdr) / BLOCK_SIZE] = block_size_on_level(lvl);
		allBlocks[((char*)_second - (char*)startAdr) / BLOCK_SIZE] = block_size_on_level(lvl);
		//lvl += 1;

		addToLevel(lvl, _first);
		addToLevel(lvl, _second);

	}

	chunk* _toRetrun = freeBlocks[lvl];
	int _index = ((char*)freeBlocks[lvl] - (char*)startAdr) / (sizeof(char)*BLOCK_SIZE); // bitno!!!
	allBlocks[_index] = -1;
	if (freeBlocks[lvl])
		if (freeBlocks[lvl]->next)
				freeBlocks[lvl]->next->prev = nullptr;
				freeBlocks[lvl] = freeBlocks[lvl]->next;

	_toRetrun->next = nullptr;



	mtx.unlock();
	return _toRetrun;


}

int Buddy::myBuddy(int index, int size){
	return (index%size == 0) ? index + size : index - size; // true->buddy desno, false->buddy levo
}


void Buddy::freeMem(void* adr, int size){
	// size je stepen dvojke
	mtx.lock();
	int ind = (char*)adr - (char*)startAdr;

	int buddy_ind = myBuddy(ind, size);
	if (buddy_ind < start_index || buddy_ind >= NUM_BLOCKS){
		mtx.unlock();
		return;
	}
	allBlocks[ind] = log2(size);
	while (buddy_ind < NUM_BLOCKS && buddy_ind >= start_index && allBlocks[ind] == allBlocks[buddy_ind]){
		// izbaciti buddy iz liste slobodnih, spojiti ih, ponovo naci buddy_ind
		chunk* _toDelete = (chunk*)((char*)startAdr + buddy_ind*BLOCK_SIZE);
		if (_toDelete->prev) _toDelete->prev->next = _toDelete->next;
		else freeBlocks[NUM_LEVELS - allBlocks[buddy_ind]] = _toDelete->next;

		if (_toDelete->next) _toDelete->next->prev = _toDelete->prev;
		_toDelete->next = nullptr;
		_toDelete->prev = nullptr;

		// spajanje sa buddy
		int ind1 = ind < buddy_ind ? ind : buddy_ind;
		int ind2 = ind1 == ind ? buddy_ind : ind;
		allBlocks[ind1] += 1;
		allBlocks[ind2] = -2;

		buddy_ind = myBuddy(ind1, pow(2, allBlocks[ind1]));
	}

	chunk* _new = (chunk*)((char*)startAdr + ind*BLOCK_SIZE);
	addToLevel(level_from_block_size(pow(2, allBlocks[ind])), _new);
	mtx.unlock();
	return;


}

void Buddy::addToLevel(int level, chunk* adr){
	adr->prev = nullptr;
	if (freeBlocks[level] != nullptr)
		freeBlocks[level]->prev = adr;
	adr->next = freeBlocks[level];
	freeBlocks[level] = adr;

}



