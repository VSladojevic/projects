#pragma once
#include "slab.h"

#include <mutex>

class Buddy{
public:
	struct chunk{
		chunk* prev = nullptr;
		chunk* next = nullptr;
	};


	void *startOfBlock(const void *mem);

	int NUM_BLOCKS;
	static const int MAX_LEVELS = 30;
	int NUM_LEVELS;

	int sysBlocks;

	void* startAdr;
	int start_index;
	void* _my_mem;

	short* allBlocks; // -2 - deo veceg, -1 - zauzet, k - komad velicine 2^k
	chunk* freeBlocks[MAX_LEVELS];
	

	std::recursive_mutex mtx;

	Buddy(void* mem, int NO_BLOCKS, int mainCache_size);
	void* takeMem(int size_blocks);
	void freeMem(void* adr, int size);

	int ptrToInd(const void * mem);
	//void* calcAddress(int ind, void* mem);
	int removeFromLevel(int lvl);

private:
	int block_size_on_level(int lvl);
	int level_from_block_size(int size);
	int myBuddy(int index, int size);
	void addToLevel(int level, chunk* adr);

};
