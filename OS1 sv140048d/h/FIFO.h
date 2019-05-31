/*
 * FIFO.h
 *
 *
 *      Author: Vladimir Sladojevic
 */

#ifndef FIFO_H_
#define FIFO_H_



struct Elem {
	int* data;
	Elem* next;

	Elem(int* d){
		data=d;
		next=NULL;
	}

};


class FIFO {
public:
	FIFO();
	~FIFO();

	void put(int* data);
	int* get();
	void ispis();

private:
	Elem* first;
	Elem* last;
};

#endif /* FIFO_H_ */
