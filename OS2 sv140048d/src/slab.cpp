#define _CRT_SECURE_NO_WARNINGS
#include "buddy.h"
#include "slab.h"

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <mutex>

using namespace std;

#define NAME_LEN (31)


typedef unsigned int kmem_bufctl_t;
#define slab_bufctl(slabp) \
         ((kmem_bufctl_t *)(((slab*)slabp)+1))

#define BUFCTL_END  ((kmem_bufctl_t)~0U) // proveriti da li vraca korektno

struct slab{
	kmem_cache_s* myCache = nullptr;
	slab* next = nullptr;
	slab* prev = nullptr;
	int active_objs = 0;
	void* startMem; // odakle pocinje prvi objekat
	kmem_bufctl_t free = 0;
};

struct kmem_cache_s{
	slab* slabs_free = nullptr;
	slab* slabs_partial = nullptr;
	slab* slabs_full = nullptr;
	int growing = 0;
	unsigned int color; // broj razlitih pomeraja
	unsigned int color_off = CACHE_L1_LINE_SIZE;
	unsigned int color_next = 0; // gde pocinje sledeci slab (0:color-1)
	unsigned int objsize;
	unsigned int num_obj; // broj objekata koji stanu na jedan slab
	unsigned int blk_order; // velicina slab-a (2^blk_order blokova)
	void(*ctor)(void *) = nullptr;
	void(*dtor)(void *) = nullptr;
	char name[NAME_LEN];
	kmem_cache_s* next = nullptr;
	kmem_cache_s* prev = nullptr;
	recursive_mutex mtx;
	unsigned short errors = 0;
};

recursive_mutex glb;

Buddy* myBuddy = nullptr;
kmem_cache_s* mainCache = nullptr;
kmem_cache_s* caches = nullptr;

kmem_cache_s** Nsizes = nullptr;


void findBestFit(kmem_cache_s* cache){ // nalazi najbolji cache->blk_order i racuna cache->num_obj
	int order = cache->blk_order = 0;
	int slab_size = pow(2, order)*BLOCK_SIZE; // velicina slaba
	int wastage = slab_size;
	wastage -= sizeof(slab);
	int est_num = wastage / (cache->objsize + sizeof(unsigned));
	int real_wastage = wastage % (cache->objsize + sizeof(unsigned));
	int wastage_perc = real_wastage / slab_size;
	while (wastage_perc > 10){
		order += 1;
		slab_size = pow(2, order)*BLOCK_SIZE;
		wastage = slab_size;
		wastage -= sizeof(slab);
		est_num = wastage / (cache->objsize + sizeof(unsigned));
		real_wastage = wastage % (cache->objsize + sizeof(unsigned));
		wastage_perc = real_wastage / slab_size;
	}

	cache->color = wastage / cache->color_off + 1;

	cache->blk_order = order;
	cache->num_obj = est_num;


}

void kmem_init(void *space, int block_num){
	myBuddy = new(space)Buddy(space, block_num, sizeof(kmem_cache_s));
	mainCache = new(myBuddy->_my_mem)  kmem_cache_s;
	mainCache->objsize = sizeof(kmem_cache_s); 
	strcpy(mainCache->name, "main cache");
	//myBuddy->startAdr = (void*)((char*)space + BLOCK_SIZE*myBuddy->sysBlocks);
	//myBuddy->start_index = myBuddy->sysBlocks;
	
	findBestFit(mainCache);

	Nsizes = new(mainCache + 1) kmem_cache_s*[28];
	for (int i = 0; i < 28; i++)
	{
		Nsizes[i] = nullptr;
	}

}

kmem_cache_t *kmem_cache_create(const char *name, size_t size,
	void(*ctor)(void *),
	void(*dtor)(void *)) // Allocate cache
{
	mainCache->mtx.lock();
	kmem_cache_s* _myCache;
	void* mem = kmem_cache_alloc(mainCache);
	_myCache = new(mem)kmem_cache_s;
	_myCache->ctor = ctor;
	_myCache->dtor = dtor;
	_myCache->objsize = size;
	strcpy(_myCache->name, name);
	findBestFit(_myCache);
	_myCache->prev = nullptr;
	if (caches == nullptr) caches = _myCache;
	else{
		caches->prev = _myCache;
		_myCache->next = caches;
		caches = _myCache;
	}

	mainCache->mtx.unlock();
	return _myCache;

}


int kmem_cache_shrink(kmem_cache_t *cachep) // Shrink cache
{
	cachep->mtx.lock();
	if (cachep->growing > 0) {
		cachep->growing = 0;
		cachep->mtx.unlock();
		return 0;
	}
	else{
		// obrisati free
		int deleted = 0;
		slab* tek = cachep->slabs_free;
		while (!tek){
			slab* tmp = tek;
			if (cachep->dtor){
				for (int i = 0; i < cachep->num_obj; i++) {
					void *objp = (char *)tmp->startMem + cachep->objsize * i;
					cachep->dtor(objp);
				}
			}
			tek = tek->next;
			myBuddy->freeMem(tmp, pow(2,cachep->blk_order));
			deleted++;
		}
		cachep->slabs_free = nullptr;

		cachep->mtx.unlock();
		return deleted;
	}
}

void* getObjectFromSlab(slab* _slab){
	kmem_cache_s* _slabCache = _slab->myCache;
	void* objp = (char*)_slab->startMem + _slab->free*_slabCache->objsize;
	_slab->free = slab_bufctl(_slab)[_slab->free];
	_slab->active_objs += 1;
	return objp;
}

slab* createNewSlab(kmem_cache_s* cachep){
	slab* _new = (slab*)myBuddy->takeMem(pow(2, cachep->blk_order));
	
	if (_new == nullptr){
		cachep->errors |= 1;
		system("pause");
		kmem_cache_error(cachep);
		system("pause");
		exit(1);
		return nullptr;
	}
	_new = new(_new)slab;
	//kmem_bufctl_t* freeSlots = (kmem_bufctl_t*)(_new + 1);
	kmem_bufctl_t* freeSlots = slab_bufctl(_new);
	_new->startMem = (char*)(freeSlots + cachep->num_obj) + cachep->color_off*cachep->color_next;
	cachep->color_next = (cachep->color_next + 1) % cachep->color;
	for (unsigned int i = 0; i < cachep->num_obj - 1; i++)
	{
		freeSlots[i] = i + 1;
	}
	freeSlots[cachep->num_obj - 1] = BUFCTL_END;

	return _new;
}

void moveSlab(slab* _slab, slab* &source, slab* &dest){
	//if (!source || !dest || !_slab) return;
	_slab->myCache->mtx.lock();
	if (_slab->prev == nullptr) source = source->next;
	if (_slab->prev) _slab->prev->next = _slab->next;
	if (_slab->next) _slab->next->prev = _slab->prev;
	_slab->prev = nullptr;
	_slab->next = dest;
	if (dest) dest->prev = _slab;
	dest = _slab;
	_slab->myCache->mtx.unlock();
}

void *kmem_cache_alloc(kmem_cache_t *cachep) // Allocate one object from cache
{
	
	void *object = nullptr;
	cachep->mtx.lock();
	if (cachep->slabs_partial != nullptr) {
		slab* _slab = cachep->slabs_partial;
		
		object = getObjectFromSlab(_slab);
		if (_slab->active_objs == cachep->num_obj)
			moveSlab(_slab, cachep->slabs_partial, cachep->slabs_full);
		
	}
	else if (cachep->slabs_free != nullptr) {
		slab* _slab = cachep->slabs_free;
		object = getObjectFromSlab(_slab);
		if (cachep->num_obj > _slab->active_objs)
			moveSlab(_slab, cachep->slabs_free, cachep->slabs_partial);
		else {
			moveSlab(_slab, cachep->slabs_free, cachep->slabs_full);
		}
	}
	else {
		slab* _slab = createNewSlab(cachep);
		if (cachep->ctor){
			for (int i = 0; i < cachep->num_obj; i++)
			{
				cachep->ctor((char*)_slab->startMem + i * cachep->objsize);
			}
		}
		_slab->myCache = cachep;
		object = getObjectFromSlab(_slab);
		if (cachep->num_obj > _slab->active_objs)cachep->slabs_partial = _slab;
		else {
			_slab->next = cachep->slabs_full;
			if (cachep->slabs_full)
				cachep->slabs_full->prev = _slab;

			cachep->slabs_full = _slab;
		}
		cachep->growing = 1;
	}
	cachep->mtx.unlock();
	return object;
}


slab* slabFromObject(kmem_cache_t *cachep, void *objp)
{
	// pocetak bloka je pocetak deskriptora slab-a, tj pocetak slab-a
	return (slab*)myBuddy->startOfBlock(objp);
}

void kmem_cache_free(kmem_cache_t *cachep, void *objp) // Deallocate one object from cache
{
	// odrediti kom slab-u pripada *objp
	slab* objSlab = slabFromObject(cachep, objp);
	if (objSlab==nullptr) return;
	if (cachep != objSlab->myCache) { 
		// greska 
		cachep->errors |= 2;  
		objSlab->myCache->errors |= 2;  
		return;
	}
	cachep->mtx.lock();
	if (objSlab->myCache->dtor){
		objSlab->myCache->dtor(objp);
	}
	
	// dodati ga u free niz
	int ind = (char*)objp - (char*)objSlab->startMem;
	ind /= cachep->objsize;
	slab_bufctl(objSlab)[ind] = objSlab->free;
	objSlab->free = ind;

	// premestiti slab po potrebi
	bool pun = false;
	if (objSlab->active_objs == cachep->num_obj) pun = true;
	objSlab->active_objs -= 1;
	if (objSlab->active_objs > 0){
		if (pun){
			moveSlab(objSlab, cachep->slabs_full, cachep->slabs_partial);
		}
		// else - ostaje na partial
	}
	else{
		// ide na free
		if (pun){
			// u slucaju da je kapacitet slaba 1 objekat
			moveSlab(objSlab, cachep->slabs_full, cachep->slabs_free);
		}
		else{
			moveSlab(objSlab, cachep->slabs_partial, cachep->slabs_free);
		}
	}

	cachep->mtx.unlock();

}


void *kmalloc(size_t size) // Alloacate one small memory buffer
{
	glb.lock();
	int order = log2(size);
	int ind = order - 5;
	if (!Nsizes[ind]){
		Nsizes[ind] = kmem_cache_create("buffer" + size, size, nullptr, nullptr);
	}

	
	void* ret = kmem_cache_alloc(Nsizes[ind]);
	glb.unlock();
	return ret;

}

void kfree(const void *objp) // Deallocate one small memory buffer
{
	if (!objp) return;
	slab* objSlab = (slab*)myBuddy->startOfBlock(objp);
	kmem_cache_s* cachep = objSlab->myCache;
	if (objSlab == nullptr) return;
	cachep->mtx.lock();
	
	// dodati ga u free niz
	int ind = (char*)objp - (char*)objSlab->startMem;
	ind /= cachep->objsize;
	slab_bufctl(objSlab)[ind] = objSlab->free;
	objSlab->free = ind;

	// premestiti slab po potrebi
	bool pun = false;
	if (objSlab->active_objs == cachep->num_obj) pun = true;
	if (--objSlab->active_objs > 0){
		if (pun){
			moveSlab(objSlab, cachep->slabs_full, cachep->slabs_partial);
		}
		// else - ostaje na partial
	}
	else{
		// ide na free
		if (pun){
			// u slucaju da je kapacitet slaba 1 objekat
			moveSlab(objSlab, cachep->slabs_full, cachep->slabs_free);
		}
		else{
			moveSlab(objSlab, cachep->slabs_partial, cachep->slabs_free);
		}
	}
	cachep->mtx.unlock();

}

void kmem_cache_destroy(kmem_cache_t *cachep) // Deallocate cache
{
	if (!cachep) return;
	cachep->mtx.lock();
	if (cachep->slabs_free){
		slab* tek = cachep->slabs_free;
		while (!tek){
			slab* tmp = tek;
			if (cachep->dtor){
				for (int i = 0; i < cachep->num_obj; i++) {
					void *objp = (char *)tmp->startMem + cachep->objsize * i;
					cachep->dtor(objp);
				}
			}
			tek = tek->next;
			myBuddy->freeMem(tmp, pow(2, cachep->blk_order));			
		}
		cachep->slabs_free = nullptr;
	}
	if (cachep->slabs_partial){
		slab* tek = cachep->slabs_partial;
		while (!tek){
			slab* tmp = tek;
			if (cachep->dtor){
				for (int i = 0; i < cachep->num_obj; i++) {
					void *objp = (char *)tmp->startMem + cachep->objsize * i;
					cachep->dtor(objp);
				}
			}
			tek = tek->next;
			myBuddy->freeMem(tmp, pow(2, cachep->blk_order));			
		}
		cachep->slabs_partial = nullptr;
	}
	if (cachep->slabs_full){
		slab* tek = cachep->slabs_full;
		while (!tek){
			slab* tmp = tek;
			if (cachep->dtor){
				for (int i = 0; i < cachep->num_obj; i++) {
					void *objp = (char *)tmp->startMem + cachep->objsize * i;
					cachep->dtor(objp);
				}
			}
			tek = tek->next;
			myBuddy->freeMem(tmp, pow(2, cachep->blk_order));
		}
		cachep->slabs_full = nullptr;
	}
	
	// PROVERITI !!!
	if (cachep->prev) cachep->prev->next = cachep->next;
	if (cachep->next) cachep->next->prev = cachep->prev;
	cachep->next = cachep->prev = nullptr;
	
	cachep->mtx.unlock();
	kmem_cache_free(mainCache, cachep);

}

void kmem_cache_info(kmem_cache_t *cachep) // Print cache info
{
	glb.lock();
	int no_slabs = 0;
	int total_objs = 0;
	slab* tmp = cachep->slabs_free;
	while (tmp){
		no_slabs++;
		tmp = tmp->next;
	}
	tmp = cachep->slabs_full;
	while (tmp){
		total_objs += tmp->active_objs;
		no_slabs++;
		tmp = tmp->next;
	}
	tmp = cachep->slabs_partial;
	while (tmp){
		total_objs += tmp->active_objs;
		no_slabs++;
		tmp = tmp->next;
	}

	double p = 100*(cachep->objsize)*total_objs / (pow(2, cachep->blk_order)*no_slabs*BLOCK_SIZE);

	cout << cachep->name<<", obj size:"<<cachep->objsize<<", cache size:"<<pow(2,cachep->blk_order)*no_slabs<<", slabs:"<<
		no_slabs<<", objects in slab:"<<cachep->num_obj<<", "<< p <<"% full"<< endl;
	glb.unlock();
	/*ime, velièina jednog podatka izražena u
	bajtovima, velièina celog keša izraženog u broju blokova, broj ploèa, broj objekata u jednoj
		ploèi i procentualna popunjenost keša*/
}

int kmem_cache_error(kmem_cache_t *cachep) // Print error message
{

	glb.lock();
	if (!cachep) {
		cout << "Cache doesn't exist!" << endl;
		glb.unlock();
		return -1;
	}
	cout << "Cache: " << cachep->name << endl;
	for (int i = 0; i < 2; i++)
	{
		if (cachep->errors&i) {
			switch (i) {
			case(0) :
				cout << "Error not enough memory" << endl;
				break;
			case(1) :
				cout << "Error deleting objects, different cache pointers" << endl;
				break;
			}

		}
	}
	if (cachep->errors) {
		glb.unlock();
		return 1;
	}
	else {
		glb.unlock();
		return 0;
	}
}
