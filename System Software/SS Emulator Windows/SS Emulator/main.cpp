#include <stdio.h>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "Structures.h"
#include "Emulator.h"

using namespace std;

void raiseError(string message){
	cout << message << "\n";
	getchar();
	exit(-1);
}

void trim(string& line) {
	int firstIndex = line.find_first_not_of(" \t");
	// za linux 
	// int firstIndex = line.find_first_not_of(" \t\r");
	if (firstIndex > -1) {
		line = line.substr(firstIndex);
		int lastIndex = line.find_last_not_of(" \t");
		// za linux 
		// int firstIndex = line.find_first_not_of(" \t\r");
		line = line.substr(0, lastIndex + 1);
	}
	else {
		line = "";
	}
}

string toHex(int decimalValue, int bytes) {
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

regex regexSymbolTable(R"(^(\d+)\s+(.?\w+)\s+(\w+)\s+([0-9a-f]+)\s+([lg])$)");
regex regexRealloc(R"(^([0-9a-f]+)\s+(\w+)\s+(\d+)$)");

SingleFile parseInputFile(string path) {
	ifstream inputStream;
	inputStream.open(path);
	if (!inputStream) {
		raiseError("File " + path + " can't be opened");
	}
	
	string line;
	SingleFile file;

	while (!inputStream.eof()) {
		getline(inputStream, line);
		trim(line);
		if (line == "") {
			continue;
		}

		/* 
		   tri opcije
		   #SymbolTable
		   #.rel.sekcija
		   #.sekcija 
		*/
 
		if (line.find("#SymbolTable")!=-1) {
			getline(inputStream, line);
			smatch match;
			getline(inputStream, line);
			trim(line);
			string s, symbolName;
			int offset;
			while (line != "") {
				// no, symbol, section, offset, visibility
				if (!regex_search(line, match, regexSymbolTable)) {
					raiseError("Invalid syntax for symbol table, file " + path);
				}
				s = match[4];
				offset = strtol(s.c_str(), NULL, 16);
				Symbol sym = Symbol(stoi(match[1]), match[2], match[3], offset, match[5]);
				file.symbolTable.push_back(sym);
				symbolName = match[2];
				if (symbolName == "UND") {
					file.startLocation = offset;
				}
				if (symbolName[0] == '.') {
					// sekcija je, treba je ubaciti
					Section s;
					s.name = symbolName.substr(1);
					s.startAddress = offset;
					file.sections[s.name] = s;
				}
				getline(inputStream, line);
				trim(line);
			} 
		}
		else if (line.find("#.rel") != -1) {
			string name = line.substr(line.find_last_of(".") + 1);
			getline(inputStream, line);
			smatch match;
			string tmp;
			getline(inputStream, line);
			trim(line);
			while (line != "") {
				// offset, type, no
				if (!regex_search(line, match, regexRealloc)) {
					raiseError("Invalid syntax for reallocation record, file " + path);
				}
				Realloc r = Realloc();
				tmp = match[1];
				r.offset = strtol(tmp.c_str(), NULL, 16);
				r.type = match[2];
				r.numSymbol = stoi(match[3]);
				r.section = name;
				file.reallocs.push_back(r);
				getline(inputStream, line);
				trim(line);
			} 

		}
		else if (line.find("#.") != -1) {
			string name = line.substr(line.find_first_of(".") + 1);
			getline(inputStream, line);
			trim(line);
			file.sections[name].content = "";
			for (char c : line) {
				if (c != ' ') {
					file.sections[name].content += c;
				}
			}
			int size = file.sections[name].content.length() / 2;
			file.sections[name].size = size;
			file.size += size;

		}
		else {
			raiseError("Invalid input file syntax, file " + path);
		}

	}


	inputStream.close();
	return file;
}


int main(int argc, char* argv[]) {
	// argv[0] - naziv programa
	// argv[i>0] - putanja do fajla
	// mora biti zadat bar jedan fajl
	if (argc < 2) {
		cout << "Mora se zadati bar jedan predmetni fajl \r\n";
		return 0;
	}
	vector<SingleFile> allFiles;
	for (int i = 1; i < argc; i++) {
		string filePath = argv[i];
		SingleFile f = parseInputFile(filePath);
		allFiles.push_back(f);
	}

	int aStart, bStart, aSize, bSize;
	const int max = 65536;
	for (int i = 0; i < allFiles.size()-1; i++) {
		aStart = allFiles[i].startLocation;
		aSize = allFiles[i].size; 
		if (aStart + aSize > max) {
			raiseError("Exceeding memory limit of 2^16");
		}
		for (int j = i + 1; j < allFiles.size(); j++)
		{
			bStart = allFiles[j].startLocation;
			bSize = allFiles[j].size;
			if (bStart < aStart + aSize && aStart < bStart + bSize) {
				raiseError("Overlapping files");
			}
		}
	}

	// provera da li ima preklapanja simbola
	// i da li je definisan simbol start
	map<string, int> globalSymbols;
	// ime simbola - njegova adresa
	for (SingleFile f : allFiles) {
		for (Symbol s : f.symbolTable) {
			if (s.visibility == "g") {
				if (!globalSymbols.count(s.name)) {
					if (s.section == "undefined") {
						globalSymbols[s.name] = -1;
					}
					else {
						string symbolSection = s.section;
						for (Symbol sec : f.symbolTable) {
							if (sec.name == "." + symbolSection) {
								globalSymbols[s.name] = s.offset + sec.offset;
								break;
							}
						}
					}
				}
				else {
					// vec je dodat, ako je -1 znaci da je bio uvozen
					if (globalSymbols[s.name] == -1 && s.section != "undefined") {
						string symbolSection = s.section;
						for (Symbol sec : f.symbolTable) {
							if (sec.name == "." + symbolSection) {
								globalSymbols[s.name] = s.offset + sec.offset;
								break;
							}
						}
					}
					else if (globalSymbols[s.name] == -1 && s.section == "undefined") {
						continue;
					}
					else if (globalSymbols[s.name] != -1 && s.section == "undefined") {
						continue;
					}
					else if (globalSymbols[s.name] != -1 && s.section != "undefined") {
						raiseError("Symbol " + s.name + " has multiple definitions");
					}
					else {
						raiseError("Error@Symbol " + s.name + ", unexpected behaviour");
					}
				}
			}
		}
	}

	map<string, int>::iterator it;
	for (it = globalSymbols.begin(); it != globalSymbols.end(); it++)
	{
		if (it->second == -1) {
			raiseError("Symbol " + it->first + " not defined");
		}
	}
	

	if (!globalSymbols.count("START")) {
		raiseError("Symbol START not defined");
	}

	int pcStartAddress = globalSymbols["START"];

	// srediti realokacije i potom ucitati sadrzaj u memoriju

	Emulator emulator;
	char(&mem)[max][2] = Emulator::mem;
	for (int i = 0; i < max; i++)
	{
		mem[i][0] = '0';
		mem[i][1] = '0';
	}

	map<string, Section>::iterator itS;
	for (SingleFile sf : allFiles) {
		for (itS = sf.sections.begin(); itS != sf.sections.end(); itS++)
		{
			int start = itS->second.startAddress;
			int size = itS->second.size;
			string& content = itS->second.content;
			for (int j = start; j < start + size; j++)
			{
				mem[j][0] = content[2 * (j - start)];
				mem[j][1] = content[2 * (j - start) + 1];
			}
		}
	}


	// vrednosti Realloc offset-a u SingleFile su prebacene u dekadne!
	int startAdr;
	int sectionStart;
	int len;
	Symbol s;
	map<string, int> typeToLen = { { "R_386_8", 1 }, { "R_386_32", 4 }, { "R_386_16", 2 }, { "R_386_PC16", 2 } };
	for (SingleFile f : allFiles) {
		for (Realloc r : f.reallocs) {
			sectionStart = f.sections[r.section].startAddress;
			startAdr = f.sections[r.section].startAddress + r.offset;
			// koliko sledecih bajtova menjamo
			len = typeToLen[r.type];
			s = f.symbolTable[r.numSymbol];
			string tmp(2 * len, '0');
			for (int i = startAdr; i < startAdr + len; i++) {
				tmp[2 * (len - (i - startAdr)) - 2] = mem[i][0];
				tmp[2 * (len - (i - startAdr)) - 1] = mem[i][1];
			}
			// da li ce lepo konvertovati ako je 1, 2 i 4 bajta
			int dec = strtol(tmp.c_str(), NULL, 16);
			int increment = 0;
			if (s.name[0] == '.') {
				// sekcija
				increment = s.offset;
			}
			else {
				// globalni simbol
				increment = globalSymbols[s.name];
			}

			if (r.type == "R_386_PC16") {
				increment -= (sectionStart + r.offset);
			}

			dec += increment;
			string hex = toHex(dec, len);
			for (int i = startAdr; i < startAdr + len; i++) {
				mem[i][0] = hex[2 * (len - (i - startAdr)) - 2];
				mem[i][1] = hex[2 * (len - (i - startAdr)) - 1];
			}
		}
	}


	if (!globalSymbols.count("timerInterrupt") || !globalSymbols.count("keyboardInterrupt")) {
		raiseError("U ulaznim fajlovima nisu prepoznati simboli timerInterrupt i keyboardInterrupt");
	}

	emulator.timerIntAddress = globalSymbols["timerInterrupt"];
	emulator.keyboardIntAddress = globalSymbols["keyboardInterrupt"];


	emulator.initialize();

	// bitan redosled!
	emulator.pc = pcStartAddress;
	
	
	// emulator.printMemory(100);

	emulator.emulate();

	// emulator.printMemory(36);

	return 0;
}