#pragma once
// ne moraju pragme
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <ctime>
#include <thread>
#include <conio.h>
// za linux
// bez conio
// #include <termios.h> // ne treba, nije korisceno
// #include <pthread.h>
// #include <ncurses.h>

using namespace std;

typedef void func();


class Emulator {
public:
	static const int maxSize = 65536;
	static const int noRegisters = 8;

	// i-ti bit govori da li je i-ti interrupt setovan
	// odgovara ulazu u IVT tabelu koji treba pozvati 
	// ako je bit setovan
	// 0 -
	// 1 - timer na 1s
	// 2 - nekorektna instrukcija
	// 3 - za ispis na tastaturu
	static bool interrupts[4];

	int registers[noRegisters];
	// r7
	int& pc = registers[7];

	// r6
	int sp;

	// 16ti maskiranje prekida, 13ti prekidna rutina ulaz 1
	// najniza 4 bita ncoz
	int psw;
	int dstReg, srcReg;
	int pomReg;

	// jer je jedan bajt dva karaktera (npr "4f")
    static char mem[maxSize][2];
	

	static volatile bool running;
	static volatile bool print;
	static volatile bool inInterrupt;
	int opcode;

	short condition, ins;
	short dstAdr, dst, srcAdr, src;

	// registar u kome se cuva adresa na koju treba upisati vrednost iz pomReg,
	// u slucaju memorijskog/regindpom adresiranja
	int adr;
	// da bi kod jmp $label radilo za negativan pomeraj
	short int extraValue;

	void initialize();
	void emulate();
	void decodeInstruction();
	bool conditionSatisfied();
	void handleInterrupt();
	int getOperand(int opAdr, int op);
	void setPSWFlags();
	void storeResult();
	static string toHex(int decimalValue, int bytes);
	void printMemory(int length);
	
	static void timer();
	static void keyboard();
	static void printListener();

	void setupIVT();
	void setIVTEntry(int entryNo, int address);
	void callInterrupt(int n);

	int timerIntAddress;
	int keyboardIntAddress;
  };