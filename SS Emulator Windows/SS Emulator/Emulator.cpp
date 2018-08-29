#include "Emulator.h"

volatile bool Emulator::running;
volatile bool Emulator::print;
volatile bool Emulator::inInterrupt;

bool Emulator::interrupts[] = { false, false, false, false };
char Emulator::mem[maxSize][2];


void Emulator::initialize() {
	
	for (int i = 0; i < noRegisters; i++) {
		registers[i] = 0;
	}
	// zauzeta lokacija, posto nijedna nije zauzeta na pocetku,
	// onda stavljamo na maxSize
	sp = maxSize - 20;
	psw = 0;
	dstReg = 0;
	srcReg = 0;
	pomReg = 0;

	// postavljanje prekida
	// IVT 8 ulaza, po 2 bajta
	
	// pokretanje niti za keyboard, timer-a, flegovi, ostale stvari
	running = true;
	print = false;
	// ako je false, slobodan je
	inInterrupt = false;

	setupIVT();
	// regIn = mem[0][0];

}

void Emulator::setupIVT() {
	// 8 ulaza po 2B
	setIVTEntry(1, timerIntAddress);
	setIVTEntry(3, keyboardIntAddress);
}

void Emulator::setIVTEntry(int entryNo, int address) {
	string hex = toHex(address, 2);
	mem[entryNo][0] = hex[0];
	mem[entryNo][1] = hex[1];
	mem[entryNo + 1][0] = hex[2];
	mem[entryNo + 1][1] = hex[3];
}

void Emulator::callInterrupt(int n) {
	inInterrupt = true;
	string tmp = "";
	tmp = tmp + mem[n][0] + mem[n][1];
	tmp = tmp + mem[n+1][0] + mem[n+1][1];
	int address = strtol(tmp.c_str(), NULL, 16);

	// push pc
	sp -= 2;
	string hex = toHex(pc, 2);
	mem[sp + 1][0] = hex[0];
	mem[sp + 1][1] = hex[1];
	mem[sp][0] = hex[2];
	mem[sp][1] = hex[3];

	// push psw
	sp -= 2;
	hex = toHex(psw, 2);
	mem[sp + 1][0] = hex[0];
	mem[sp + 1][1] = hex[1];
	mem[sp][0] = hex[2];
	mem[sp][1] = hex[3];

	// call interrupt routine
	pc = address;

}

void Emulator::setPSWFlags() {
	if (pomReg < 0) {
		psw = psw | 8;
	}
	else if (pomReg == 0) {
		psw = psw | 1;
	}
}

void Emulator::storeResult() {
	if (dstAdr == 0 && dst == 7) {
			// psw
			psw = pomReg;
			return;
		}
	if (dstAdr == 1) {
		// regdir
		registers[dst] = pomReg;
		return;
	}
	// memdir ili regindpom
	string hex = toHex(pomReg, 2);
	mem[adr + 1][0] = hex[0];
	mem[adr + 1][1] = hex[1];
	mem[adr][0] = hex[2];
	mem[adr][1] = hex[3];

}

void Emulator::emulate() {
	
	/*
	static struct termios oldt, newt;
	tcgetattr( STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);
	*/
	
	/*
	initscr();
	scrollok(stdscr,TRUE);
	//keypad(stdscr,TRUE);
	//idlok(stdscr,TRUE);
	nodelay(stdscr, TRUE);
  	noecho();
	
	pthread_t thread1;
	const char *message1 = "Thread 1";
	pthread_create( &thread1, NULL, timer, (void*) message1);
	
	pthread_t thread2;
	const char *message2 = "Thread 2";
	pthread_create( &thread2, NULL, keyboard, (void*) message2);
	
	pthread_t thread3;
	const char *message3 = "Thread 3";
	pthread_create( &thread3, NULL, printListener, (void*) message3);
	
	*/

    // za linux skloniti ove threadove ispod, ubaciti ovo gore
	thread threadTimer(timer);
	thread threadKeyboard(keyboard);
	thread threadPrint(printListener);
	
	while (running) {
		decodeInstruction();
		// sad stvarno izvrsavamo instrukciju
		string hex = "", tmp = "";
		int dec = 0;
		switch (ins)
		{
		case 0:
			// add
			pomReg = dstReg + srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 1:
			// sub
			pomReg = dstReg - srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 2:
			// mul
			pomReg = dstReg * srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 3:
			// div
			pomReg = dstReg / srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 4:
			// cmp
			pomReg = dstReg - srcReg;
			setPSWFlags();
			break;

		case 5:
			// and
			pomReg = dstReg & srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 6:
			// or
			pomReg = dstReg | srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 7:
			// not
			pomReg = ~srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 8:
			// test
			pomReg = dstReg & srcReg;
			setPSWFlags();
			break;

		case 9:
			// push 
			sp -= 2;
			hex = toHex(srcReg, 2);
			mem[sp + 1][0] = hex[0];
			mem[sp + 1][1] = hex[1];
			mem[sp][0] = hex[2];
			mem[sp][1] = hex[3];
			break;

		case 10:
			// pop
			tmp = "";
			tmp = tmp + mem[sp+1][0] + mem[sp+1][1];
			tmp = tmp + mem[sp][0] + mem[sp][1];
			dec = strtol(tmp.c_str(), NULL, 16);
			pomReg = dec;
			sp += 2;
			storeResult();
			break;

		case 11:
			// call
			// push pc
			sp -= 2;
			hex = toHex(pc, 2);
			mem[sp + 1][0] = hex[0];
			mem[sp + 1][1] = hex[1];
			mem[sp][0] = hex[2];
			mem[sp][1] = hex[3];
			// pc=src
			pc = srcReg;
			break;

		case 12:
			// iret
			// pop psw
			tmp = "";
			tmp = tmp + mem[sp+1][0] + mem[sp+1][1];
			tmp = tmp + mem[sp][0] + mem[sp][1];
			dec = strtol(tmp.c_str(), NULL, 16);
			pomReg = dec;
			dstAdr = 0; 
			dst = 7;
			sp += 2;
			storeResult();
			// pop pc
			tmp = "";
			tmp = tmp + mem[sp+1][0] + mem[sp+1][1];
			tmp = tmp + mem[sp][0] + mem[sp][1];
			dec = strtol(tmp.c_str(), NULL, 16);
			pomReg = dec;
			dstAdr = 1;
			dst = 7;
			sp += 2;
			storeResult();
			inInterrupt = false;
			print = true;
			break;

		case 13:
			// mov
			pomReg = srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 14:
			// shl
			pomReg = dstReg << srcReg;
			setPSWFlags();
			storeResult();
			break;

		case 15:
			// shr
			pomReg = dstReg >> srcReg;
			setPSWFlags();
			storeResult();
			break;

		default:
			interrupts[2] = true;
			cout << "Error, unrecognized instruction" << "\n";
			getchar();
			exit(-1);
			break;

		}
		// end of switch
		// neko je generisao zahtev
		handleInterrupt();
	}
	// end of while
	
	// za linux izbaciti ovo
	threadTimer.join();
	threadKeyboard.join();
	threadPrint.join();
	
	// ubaciti ovo
	/*
	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	pthread_join( thread3, NULL);
		
	// vracanje podesavanja
	//tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	nodelay(stdscr, FALSE);

	getch();
	endwin();
	*/
	
}

bool Emulator::conditionSatisfied() {
	// ncoz

	// al
	if (condition == 3) return true;
	// gt
	if (condition == 2) {
		// n!=1 && z!=1
		return (psw & 9) == 0;
	}
	// ne
	if (condition == 1) {
		// n==1
		return psw & 8;
	}
	// eq
	// z==1
	return psw & 1;

}

int Emulator::getOperand(int opAdr, int op) {
	int operand;
	if (opAdr == 0) {
		// neposredno
		if (op == 7) {
			// psw
			operand = psw;
		}
		else {
			operand = extraValue;
		}
	}
	else if (opAdr == 1) {
		// regdir
		return registers[op];
	}
	else if (opAdr == 2) {
		// mem
		adr = extraValue;
		string tmp = "";
		// citamo podatak, pa treba obrnuti
		tmp = tmp + mem[adr + 1][0] + mem[adr + 1][1];
		tmp = tmp + mem[adr][0] + mem[adr][1];
		int dec = strtol(tmp.c_str(), NULL, 16);
		operand = dec;
	}
	else if (opAdr == 3) {
		// regindpom
		int pom = extraValue;
		adr = registers[op] + pom;
		string tmp = "";
		// citamo podatak pa treba obrnuti
		tmp = tmp + mem[adr + 1][0] + mem[adr + 1][1];
		tmp = tmp + mem[adr][0] + mem[adr][1];
		int dec = strtol(tmp.c_str(), NULL, 16);
		operand = dec;
	}
	else {
		cout << "Error, unrecognized address type" << "\n";
		getchar();
		exit(-1);
	}
	return operand;
}

void Emulator::decodeInstruction() {
	// citamo dva bajta od pc adrese
	string bytes = "";
	// prvi bajt
	bytes = bytes + mem[pc][0] + mem[pc][1];
	// drugi bajt
	bytes = bytes + mem[pc + 1][0] + mem[pc + 1][1];

	// prva 2 bita - uslov, sledeca 4 - instrukcija
	int decimal = stoi(bytes, NULL, 16);
	condition = decimal >> 14;
	ins = 0x0F & (decimal >> 10);

	dst = (decimal >> 5) & 0x1F;
	dstAdr = dst >> 3;
	dst = dst & 0x07;

	src = decimal & 0x1F;
	srcAdr = src >> 3;
	src = src & 0x07;

	/*
		da li je potrebno da procitamo jos 2 bajta instrukcije?
		kada je potrebno?
		- kada je instrukcija koja ima dst i src, a nisu oba regdir
		- kada je instrukcija koja ima samo dst ili src, a nije regdir
		tj. ako je srcAdr!=regdir || dstAdr!=regdir
	*/
	pc += 2;
	bool pswDst = (dst == 0) && (dstAdr == 7);
	bool pswSrc = (src == 0) && (srcAdr == 7);
	if (!(pswSrc || srcAdr == 1) || !(pswDst || dstAdr == 1)) {
		string extraBytes = "";
		// odmah obrnemo bajtove
		extraBytes = extraBytes + mem[pc + 1][0] + mem[pc + 1][1];
		extraBytes = extraBytes + mem[pc][0] + mem[pc][1];
		extraValue = stoi(extraBytes, NULL, 16);
		pc += 2;
	}

	
	if (conditionSatisfied()) {
		// dohvatamo podatke u registre
		if (ins <= 8 || ins >= 13) {
			// imaju i dst i src
			
			// prvo src, pa dst, da bi nakon dst u adr bila adresa gde treba da se smesti !!!
			srcReg = getOperand(srcAdr, src);
			dstReg = getOperand(dstAdr, dst);
		}
		else if (ins==9 || ins==11) {
			// samo src
			srcReg = getOperand(srcAdr, src);
		}
		else if (ins == 10) {
			// samo dst
			// ali nije potrebno da dohvatamo operand
			// dstReg = getOperand(dstAdr, dst);
		}
		else {
			// nista
		}
	}
	


}

void Emulator::printMemory(int length) {
	for (int i = 0; i < length; i++)
	{
		cout << i << ": " << mem[i][0] << mem[i][1] << "\n";
	}
	getchar();
}


string Emulator::toHex(int decimalValue, int bytes) {
	stringstream ss;
	ss << std::hex << decimalValue;
	string res(ss.str());
	// ako je -1, onda tacno taj broj koliko je dugacak
	if (bytes == -1) return res;

	// width je koliko umnozaka od 4 bita
	int width = bytes * 2;
	int length = res.length();
	int diff;
	if (length > width) {
		diff = length - width;
		return res.substr(diff);
	}
	else if (length < width) {
		diff = width - length;
		for (int i = 0; i < diff; i++) {
			res = "0" + res;
		}
		return res;
	}
	else {
		return res;
	}
}


void Emulator::handleInterrupt() {
	if (!inInterrupt) {
		if (interrupts[1]) {
			// timer
			callInterrupt(1);
			interrupts[1] = false;
		}
		if (interrupts[3]) {
			// keyboard
			callInterrupt(3);
			interrupts[3] = false;
		}
	}
}


void Emulator::keyboard() {
	while (running) {
		char key;
		
		// za linux
		// key = getchar();
		
		key = _getch();
		int val = key;

		int adr = 0x0FFC;
		string hex = toHex(val, 2);
		mem[adr + 1][0] = hex[0];
		mem[adr + 1][1] = hex[1];
		mem[adr][0] = hex[2];
		mem[adr][1] = hex[3];

		// ceka red
		interrupts[3] = true;
		

		// print = true;
	}
}

void Emulator::timer() {
	while (running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		interrupts[1] = true;
	}
}

void Emulator::printListener() {
	while (running) {
		while (!print) {}

		// print = true
		string tmp = "";
		int adr = 0x0FFE;

		// citamo podatak pa treba obrnuit
		tmp = tmp + mem[adr + 1][0] + mem[adr + 1][1];
		tmp = tmp + mem[adr][0] + mem[adr][1];
		int dec = strtol(tmp.c_str(), NULL, 16);
		
		char c = dec;
		cout << c;
		print = false;
	}
}