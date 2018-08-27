#pragma once
#include <string>
#include <vector>
using namespace std;

struct Symbol {
	int entryNumber;
	string name;
	string section;
	int offset;
	string visibility;

	Symbol(int EntryNumber = 0, string Name = "", string Section = "", int Offset = 0, string Visibility = "l") {
		name = Name;
		section = Section;
		offset = Offset;
		visibility = Visibility;
		entryNumber = EntryNumber;
	}
};

struct Section {
	string name;
	int startAddress;
	int size = 0;
	string content = "";
};

struct Realloc {
	int offset;
	string type;
	int numSymbol;
	string section;
};

struct SingleFile {
	map<string, Section> sections;
	vector<Symbol> symbolTable;
	vector<Realloc> reallocs;
	int startLocation;
	int size = 0;
};