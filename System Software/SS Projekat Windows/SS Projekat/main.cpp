#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Realloc;
class Instruction;
struct Section;
struct Symbol;
class ExpressionSolver;


vector<string> validSections = { ".text", ".data", ".rodata", ".bss" };
string inputFileName;
string outputFileText;
string outputFileName;
int startingAddress;

int locationCounter;

int inputFileLine = 0;

vector<string> lines;

map<string, int> opcodesAdr = { { "imm", 0 }, { "regdir", 1 }, { "mem", 2 }, { "regindpom", 3 } };
map<string, int> opcodesCon = { { "eq", 0 }, { "ne", 1 }, { "gt", 2 }, { "al", 3 } };
map<string, int> opcodesIns = { { "add", 0 }, { "sub", 1 }, { "mul", 2 }, { "div", 3 }, { "cmp", 4 }, { "and", 5 }, { "or", 6 }, { "not", 7 },
{ "test", 8 }, { "push", 9 }, { "pop", 10 }, { "call", 11 }, { "iret", 12 }, { "mov", 13 }, { "shl", 14 }, { "shr", 15 } };


vector<string> reallocationTypes = { "R_386_8", "R_386_32", "R_386_16", "R_386_PC16" };


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


struct Realloc {
	Instruction* instruction;
	int offset;
	string type;
	int numSymbol;
	string section;
	Realloc(int Offset = 0, string Type = "", int NumSymbol = 0, string Section = "") {
		offset = Offset;
		type = Type;
		numSymbol = NumSymbol;
		section = Section;
	}
	Realloc(Instruction& Instruction, int Offset = 0, string Type = "", int NumSymbol = 0, string Section = "") {
		instruction = &Instruction;
		offset = Offset;
		type = Type;
		numSymbol = NumSymbol;
		section = Section;
	}
};


vector<Realloc> allReallocations;

struct Section {
	string name;
	int startAddress;
	Section(string Name = "", int StartAddress = 0) {
		name = Name;
		startAddress = StartAddress;
	}
	int size = 0;
	string content = "";

	void printRealloc() {
		stringstream out;
		auto delimiter = setw(20);
		string header = "#.rel." + name;
		bool found = false;
		for (Realloc realloc : allReallocations)
		{
			if (realloc.section == name) {
				found = true;
				break;
			}
		}
		if (!found) {
			return;
		}
		out << header << "\r\n";
		out << right << "#offset" << delimiter << "type" << setw(22) << "no\r\n";
		for (Realloc realloc : allReallocations)
		{
			if (realloc.section == name) {
				out << toHex(realloc.offset, 4) << delimiter << realloc.type << delimiter << realloc.numSymbol << "\r\n";
			}
		}
		out << "\r\n";
		outputFileText += out.str();
	}

	void printContent() {
		stringstream out;
		string header = "#." + name;
		out << header << "\r\n";
		for (int i = 0; i < content.length(); i += 2) {
			out << content[i] << content[i + 1] << " ";
		}
		outputFileText += out.str();
	}

};


Section* currentSection = nullptr;
// nije najsrecnije?
Section globalTmp;

map<string, Section> availableSections;

struct Symbol {
	string name;
	string section;
	int offset;
	char visibility;
	int entryNumber;
	Symbol(string Name = "", string Section = "", int Offset = 0, char Visibility = 'l', int EntryNumber = 0) {
		name = Name;
		section = Section;
		offset = Offset;
		visibility = Visibility;
		entryNumber = EntryNumber;
	}
};

vector<Symbol> symbolTable;


void trim(string& line) {
	int firstIndex = line.find_first_not_of(" \t");
	if (firstIndex > -1) {
		line = line.substr(firstIndex);
		int lastIndex = line.find_last_not_of(" \t");
		line = line.substr(0, lastIndex + 1);
	}
	else {
		line = "";
	}
}

void raiseError(string message){
	cout << "error at line " << inputFileLine << ": " << message << "\r\n";
	getchar();
	exit(-1);
}

void raiseError(string message, int inputFileLine){
	cout << "error at line " << inputFileLine << ": " << message << "\r\n";
	getchar();
	exit(-1);
}

class ExpressionSolver {
public:

	static string toDecimal(string match) {
		stringstream res;
		trim(match);
		if (match == "") {
			res << "";
		}
		else if (match.length() >= 2 && (match.substr(0, 2) == "0x" || match.substr(0, 2) == "0X")) {
			// treba proveriti da li je format dobar, tj samo hex cifre
			int number = strtol(match.c_str(), NULL, 16);
			res << number;
		}
		else if (match.length() >= 2 && (match.substr(0, 2) == "0b" || match.substr(0, 2) == "0B")) {
			// proveriti da li su samo 0 i 1
			int number = strtol(match.substr(2).c_str(), NULL, 2);
			res << number;
		}
		else if (match[0] >= '0' && match[0] <= '9') {
			res << match;
		}

		else {
			// labela
			string label = match;
			bool found = false;
			for (Symbol s : symbolTable)
			{
				if (s.name == label) {
					found = true;
					res << s.offset;
					break;
				}
			}
			if (!found) {
				// kada proba da izracuna izraz nece uspeti jer ce
				// biti dva znaka jedan do drugog, pa ce vratit minint kao rezultat, tj gresku
				res << "";
			}
		}
		// ako je labela treba staviti vrednost labele
		// ako nije, onda proveriti da li su samo cifre

		return res.str();
	}

	static string prepareExpression(string expression) {
		/*
		Zameni labele, binarne i heksadecimalne brojeve dekadnim,
		a whitespace-ove ukloni.
		*/
		stringstream out;
		string s = expression;
		regex operands(R"([^()+\*\/-]+)");
		s = regex_replace(s, regex("\\(-"), "(0-");
		s = regex_replace(s, regex("^-"), "0-");

		smatch matches;
		while (regex_search(s, matches, operands)) {
			out << s.substr(0, matches.position(0));
			out << toDecimal(matches[0]);
			s = matches.suffix();
		}

		// !!!
		out << s;

		return out.str();
	}

	static string toPostfix(string expression) {
		stringstream postfix;
		stack<char> opStack;
		string delimiter = " ";
		map<char, int> priorities = { { '+', 2 }, { '-', 2 }, { '*', 3 }, { '/', 3 }, { '(', 1 } };
		for (char c : expression)
		{
			if (c >= '0' && c <= '9') {
				postfix << c;
			}
			else if (c == '(') {
				opStack.push(c);
				postfix << delimiter;
			}
			else if (c == ')') {
				postfix << delimiter;
				char topOper = opStack.top();
				opStack.pop();
				while (topOper != '(') {
					postfix << delimiter;
					postfix << topOper;
					if (opStack.empty()) {
						return "5 + 3 -";
					}
					topOper = opStack.top();
					opStack.pop();
					// ako nema zagrade, onda u nekom trenutku nece imati sta da popuje,
					// tu bi trebalo error izbaciti
				}
			}
			else {
				while (!opStack.empty() && priorities[opStack.top()] >= priorities[c]) {
					postfix << delimiter;
					postfix << opStack.top();
					opStack.pop();
				}
				postfix << delimiter;
				opStack.push(c);
			}
		}

		while (!opStack.empty()) {
			postfix << delimiter;
			if (opStack.top() == '(') {
				return "5 + 3 -";
			}
			postfix << opStack.top();
			opStack.pop();
		}

		return postfix.str();
	}

	static int parseAndSolveExpression(string expression) {
		// kada budem radio second pass, tamo uraditi prepareExpression ako je moguce,
		// pa onda pozvati toPostfix i resavanje
		stack<int> operands;
		vector<string> tokens;
		stringstream out;
		string prepared = ExpressionSolver::prepareExpression(expression);
		string postfix = ExpressionSolver::toPostfix(prepared);
		regex r("\\s+");
		smatch matches;
		while (regex_search(postfix, matches, r)) {
			tokens.push_back(postfix.substr(0, matches.position(0)));
			postfix = matches.suffix();
		}

		// !!!
		tokens.push_back(postfix.substr(0, matches.position(0)));
		vector<string>::iterator it;
		int op1, op2;
		int result;

		// proci kroz tokense redom, kada se zavrsi proveriti ako nesto nije prazno onda je greska "invalid expression"
		while (!tokens.empty()) {
			it = tokens.begin();
			if (*it != "+" && *it != "-" && *it != "*" && *it != "/") {
				operands.push(stoi(*it));
			}
			else {
				if (!operands.empty()) {
					op2 = operands.top();
					operands.pop();
				}
				else {
					return std::numeric_limits<int>::min();
				}
				if (!operands.empty()) {
					op1 = operands.top();
					operands.pop();
				}
				else {
					return std::numeric_limits<int>::min();
				}

				if (*it == "+") {
					result = op1 + op2;
				}
				else if (*it == "-") {
					result = op1 - op2;
				}
				else if (*it == "*") {
					result = op1 * op2;
				}
				else {
					result = op1 / op2;
				}
				operands.push(result);

			}
			tokens.erase(it);

		}

		if (operands.size() > 1) {
			return std::numeric_limits<int>::min();
		}

		return operands.top();
	}

};

vector<Instruction> instructions;


/*
DIRECTIVE REGEXES
*/
string expr = R"([.()+\s\w\*\/-]+)";
string elem = expr + "|" + R"(-?\d+|".*"|'.'|\w+|0x[\da-f]+|0b[01]+)";
regex regexGlobal(R"(^.(global)\s+(\w+)(\s*,\s*(\w+))*$)", regex_constants::icase);
regex regexChar("^.(char)\\s+(" + elem + ")(\\s*,\\s*(" + elem + "))*$", regex_constants::icase);
regex regexWord("^.(word)\\s+(" + elem + ")(\\s*,\\s*(" + elem + "))*$", regex_constants::icase);
regex regexLong("^.(long)\\s+(" + elem + ")(\\s*,\\s*(" + elem + "))*$", regex_constants::icase);
// proveriti da li je stepen dvojke
regex regexAlign(R"(^.(align)(?:\s+(\d+))?(?:\s*,\s*(\d+|0x[\da-f]+|0b[01]+))?$)", regex_constants::icase);
regex regexSkip(R"(^.(skip)\s+(\d+)(?:\s*,\s*(\d+|0x[\da-f]+|0b[01]+))?$)", regex_constants::icase);


/*
INSTRUCTION REGEXES
*/
string dst = R"([.\w\[\]&\$*]+)";
string src = R"([.\w\[\]&\$*]+)";
regex regexAdd("^(add)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexMul("^(mul)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexSub("^(sub)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexDiv("^(div)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexCmp("^(cmp)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexAnd("^(and)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexOr("^(or)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexNot("^(not)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexTest("^(test)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexMov("^(mov)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexShl("^(shl)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);
regex regexShr("^(shr)(eq|ne|gt|al)\\s+(" + dst + ")\\s*,\\s*(" + src + ")$", regex_constants::icase);

vector<regex> instructionsArithmetic = { regexAdd, regexMul, regexSub, regexDiv, regexCmp, regexAnd, regexOr,
regexNot, regexTest, regexMov, regexShl, regexShr };

regex regexPush("^(push)(eq|ne|gt|al)\\s+(" + src + ")$", regex_constants::icase);
regex regexPop("^(pop)(eq|ne|gt|al)\\s+(" + dst + ")$", regex_constants::icase);
regex regexCall("^(call)(eq|ne|gt|al)\\s+(" + src + ")$", regex_constants::icase);
regex regexIret("^(iret)(eq|ne|gt|al)$", regex_constants::icase);
regex regexRet("^(ret)(eq|ne|gt|al)$", regex_constants::icase);
regex regexJmp(R"(^(jmp)(eq|ne|gt|al)\s+([$]?.?\w+)$)", regex_constants::icase);


regex regindpom(R"(^(r[0-7]|psw)\[(-?(?:\d+|0x[\da-f]+|0b[01]+)|[$]?\w+)\]$)", regex_constants::icase);
regex regdir(R"(^r[0-7]|psw$)", regex_constants::icase);
regex imm(R"(^-?(?:\d+|&\w+|0x[\da-f]+|0b[01]+)$)", regex_constants::icase);
regex mem(R"(^([$]?[.]?\w+|\*?(?:\d+|0x[\da-f]+|0b[01]+))$)", regex_constants::icase);
regex pcrel(R"(^\$.?\w+$)", regex_constants::icase);

map<string, regex> adrToRegex = { { "regindpom", regindpom }, { "regdir", regdir }, { "imm", imm }, { "mem", mem } };

string adrType(string operand) {
	map<string, regex>::iterator it;
	smatch match;
	if (regex_search(operand, match, regdir)) {
		return "regdir";
	}
	for (it = adrToRegex.begin(); it != adrToRegex.end(); it++)
	{
		if (regex_search(operand, match, it->second)) {
			return it->first;
		}
	}
	return "";
}

string stringToLower(string line) {
	string lower = "";
	for (char l : line)
	{
		lower += tolower(l);
	}
	return lower;
}

bool isPow2(int n) {
	if (n == 0) return 0;
	return (n & (n - 1)) == 0;
}


class Instruction {

public:
	string instruction = "";
	string condition = ""; // (eq|ne|gt|al)
	string dst = "";
	string src = "";
	string dstAdr = "";
	string srcAdr = "";
	string directive = "";
	string section = "";
	vector<string> params;
	int lineNumber;
	int locationOffset; // offset od pocetka sekcije
	// int size;

	// za jmp i ret, "vestacki" konstruktor
	Instruction(int LocationOffset, string Section, string Instruction, string Condition, string Dst, string Src, string DstAdr, string SrcAdr) {
		instruction = stringToLower(Instruction);
		condition = stringToLower(Condition);
		dst = Dst;
		src = Src;
		dstAdr = DstAdr;
		srcAdr = SrcAdr;
		section = Section;
		lineNumber = inputFileLine;
		locationOffset = LocationOffset;
	}

	Instruction(string Instruction, string Condition, string Dst = "", string Src = "", string DstAdr = "", string SrcAdr = "") {
		instruction = stringToLower(Instruction);
		condition = stringToLower(Condition);
		dst = Dst;
		src = Src;
		dstAdr = DstAdr;
		srcAdr = SrcAdr;
		section = currentSection->name;
		lineNumber = inputFileLine;
		locationOffset = locationCounter - currentSection->startAddress;
	}

	Instruction(string Directive) {
		directive = stringToLower(Directive);
		section = currentSection->name;
		lineNumber = inputFileLine;
		locationOffset = locationCounter - currentSection->startAddress;
	}

	void decodeInstruction() {
		// u .text sekciji je
		int firstByte = 0;
		int secondByte = 0;
		if (instruction == "jmp") {
			// ako je jmp $a, prevodimo kao add r7,a
			// ako je jmp a, prevodimo kao mov r7,a
			if (dst[0] == '$') {
				Instruction i = Instruction(-1, section, "add", condition, "r7", dst.substr(1), "regdir", "imm");
				i.decodeInstruction();
				Symbol s;
				bool found = false;
				for (Symbol s1 : symbolTable)
				{
					if (s1.name == dst.substr(1)) {
						found = true;
						s = s1;
						break;
					}
				}
				if (!found) {
					raiseError("unexpected error, operand symbol not found in symbolTable", lineNumber);
				}
				// else
				makeRealloc(s, true);
			}
			else {
				// samo za labele da radi
				Instruction i = Instruction(-1, section, "mov", condition, "r7", "&" + dst, "regdir", "imm");
				i.decodeInstruction();
				Symbol s;
				bool found = false;
				for (Symbol s1 : symbolTable)
				{
					if (s1.name == dst) {
						found = true;
						s = s1;
						break;
					}
				}
				if (!found) {
					raiseError("unexpected error, operand symbol not found in symbolTable", lineNumber);
				}
				makeRealloc(s, false);
			}
		}
		else if (instruction == "ret") {
			// prevodimo kao pop r7 (pop pc)
			Instruction i = Instruction(-1, section, "pop" + condition + " r7", condition, "r7", "", "regdir", "");
			i.decodeInstruction();
		}
		else {
			int opC = opcodesCon[condition];
			int opI = opcodesIns[instruction];
			if (src != "" && dst != "") {
				// ima i dst i src
				int opDst = opcodesAdr[dstAdr];
				int opSrc = opcodesAdr[srcAdr];

				// dst
				int  dst5bits = operand5bits(dst, dstAdr);
				int  src5bits = operand5bits(src, srcAdr);

				int a = dst5bits / 8;
				int b = dst5bits - 8 * a;
				firstByte = opC * 64 + opI * 4 + a;
				secondByte = b * 32 + src5bits;

				availableSections[section].content += toHex(firstByte, 1);
				availableSections[section].content += toHex(secondByte, 1);

				string operand = "";
				string operandAdr = "";
				if (dstAdr != "regdir") {
					operand = dst;
					operandAdr = dstAdr;
				}
				else if (srcAdr != "regdir") {
					operand = src;
					operandAdr = srcAdr;
				}

				if (operandAdr != "") {
					// 2 dodatna bajta
					if (operandAdr == "imm") {
						if (operand[0] == '&') {
							string label = operand.substr(1);
							bool found = false;
							Symbol s;
							for (Symbol s1 : symbolTable)
							{
								if (s1.name == label) {
									found = true;
									s = s1;
									break;
								}
							}
							if (!found) {
								raiseError("unexpected error, operand symbol not found in symbolTable", lineNumber);
							}
							// else
							makeRealloc(s, false);
						}
						else {
							if (locationOffset == -1) {
								// Znaci da je pozvano iz jmp $labela
								// ne treba jer se posle izlaska iz f-je pravi rel zapis
								return;
							}
							int imm = stoi(ExpressionSolver::toDecimal(operand));
							string hex = toHex(imm, 2);
							string higher = hex.substr(0, 2);
							string lower = hex.substr(2);
							availableSections[section].content += lower;
							availableSections[section].content += higher;
						}
					}
					else if (operandAdr == "mem") {
						// realloc zapis
						if (operand[0] == '*') {
							int imm = stoi(ExpressionSolver::toDecimal(operand.substr(1)));
							string hex = toHex(imm, 2);
							string higher = hex.substr(0, 2);
							string lower = hex.substr(2);
							availableSections[section].content += lower;
							availableSections[section].content += higher;
						}
						else if (operand[0] == '$') {
							// pcrel relokacioni
							string label = operand.substr(1);
							bool found = false;
							Symbol s;
							for (Symbol s1 : symbolTable)
							{
								if (s1.name == label) {
									found = true;
									s = s1;
									break;
								}
							}
							if (!found) {
								raiseError("unexpected error, operand symbol not found in symbolTable", lineNumber);
							}
							// else
							makeRealloc(s, true);
						}
						else if (operand[0] >= '0' && operand[0] <= '9') {
							// samo broj
							int imm = stoi(ExpressionSolver::toDecimal(operand));
							string hex = toHex(imm, 2);
							string higher = hex.substr(0, 2);
							string lower = hex.substr(2);
							availableSections[section].content += lower;
							availableSections[section].content += higher;
						}
						else {
							// labela
							bool found = false;
							Symbol s;
							for (Symbol s1 : symbolTable)
							{
								if (s1.name == operand) {
									found = true;
									s = s1;
									break;
								}
							}
							if (!found) {
								raiseError("unexpected error, operand symbol not found in symbolTable", lineNumber);
							}
							// else
							makeRealloc(s, false);
						}
					}
					else if (operandAdr == "regindpom") {
						// procitaj da li je pomeraj broj, labela, ...
						// ako treba napravi realloc zapis
						int i = operand.find_first_of('[');
						int j = operand.find_first_of(']');
						string displacement = operand.substr(i + 1, j - i - 1);
						string hex = toHex(stoi(ExpressionSolver::toDecimal(operand)), 2);
						string higher = hex.substr(0, 2);
						string lower = hex.substr(2);
						availableSections[section].content += lower;
						availableSections[section].content += higher;
					}
					else {
						raiseError("unexpected error, dst/src not recognized", lineNumber);
					}
				}
				// else nista, nema 2 dodatna bajta

			}
			else {

				string operand, operandAdr;
				int dst5bits = 0;
				int src5bits = 0;
				if (src != "") {
					src5bits = operand5bits(src, srcAdr);
					operand = src;
					operandAdr = srcAdr;
				}
				if (dst != "") {
					dst5bits = operand5bits(dst, dstAdr);
					operand = dst;
					operandAdr = dstAdr;
				}

				int a = dst5bits / 8;
				int b = dst5bits - 8 * a;
				firstByte = opC * 64 + opI * 4 + a;
				secondByte = b * 32 + src5bits;

				availableSections[section].content += toHex(firstByte, 1);
				availableSections[section].content += toHex(secondByte, 1);


				if (operandAdr != "regdir") {
					// 2 dodatna bajta
					if (operandAdr == "imm") {
						if (operand[0] == '&') {
							string label = operand.substr(1);
							bool found = false;
							Symbol s;
							for (Symbol s1 : symbolTable)
							{
								if (s1.name == label) {
									found = true;
									s = s1;
									break;
								}
							}
							if (!found) {
								raiseError("unexpected error, operand symbol not found in symbolTable", lineNumber);
							}
							// else
							makeRealloc(s, false);
						}
						else {
							int imm = stoi(ExpressionSolver::toDecimal(operand));
							string hex = toHex(imm, 2);
							string higher = hex.substr(0, 2);
							string lower = hex.substr(2);
							availableSections[section].content += lower;
							availableSections[section].content += higher;
						}
					}
					else if (operandAdr == "mem") {
						// realloc zapis
						if (operand[0] == '*') {
							int imm = stoi(ExpressionSolver::toDecimal(operand.substr(1)));
							string hex = toHex(imm, 2);
							string higher = hex.substr(0, 2);
							string lower = hex.substr(2);
							availableSections[section].content += lower;
							availableSections[section].content += higher;
						}
						else if (operand[0] == '$') {
							// pcrel relokacioni
							string label = operand.substr(1);
							bool found = false;
							Symbol s;
							for (Symbol s1 : symbolTable)
							{
								if (s1.name == label) {
									found = true;
									s = s1;
									break;
								}
							}
							if (!found) {
								raiseError("unexpected error, operand symbol not found in symbolTable", lineNumber);
							}
							// else
							makeRealloc(s, true);
						}
						else if (operand[0] >= '0' && operand[0] <= '9') {
							// samo broj
							int imm = stoi(ExpressionSolver::toDecimal(operand));
							string hex = toHex(imm, 2);
							string higher = hex.substr(0, 2);
							string lower = hex.substr(2);
							availableSections[section].content += lower;
							availableSections[section].content += higher;
						}
						else {
							// labela
							bool found = false;
							Symbol s;
							for (Symbol s1 : symbolTable)
							{
								if (s1.name == operand) {
									found = true;
									s = s1;
									break;
								}
							}
							if (!found) {
								raiseError("unexpected error, operand symbol not found in symbolTable", lineNumber);
							}
							// else
							makeRealloc(s, false);
						}
					}
					else if (operandAdr == "regindpom") {
						// procitaj da li je pomeraj broj, labela, ...
						// ako treba napravi realloc zapis
						int i = operand.find_first_of('[');
						int j = operand.find_first_of(']');
						string displacement = operand.substr(i + 1, j - i - 1);
						string hex = toHex(stoi(ExpressionSolver::toDecimal(operand)), 2);
						string higher = hex.substr(0, 2);
						string lower = hex.substr(2);
						availableSections[section].content += lower;
						availableSections[section].content += higher;
					}
					else {
						// iretal
						// raiseError("unexpected error, dst/src not recognized", lineNumber);
					}
				}
				else {
					// nista
				}
			}
		}
	}

	int operand5bits(string& operand, string& operandAdr) {
		int i = operand.find_first_of('[');
		if (operandAdr == "regdir") {
			if (stringToLower(operand) == "psw") {
				return 0 * 8 + 7;
			}
			else {
				return 1 * 8 + stoi(operand.substr(1, 1));
			}
		}
		else if (operandAdr == "imm") {
			return 0;
		}
		else if (operandAdr == "regindpom") {
			// 
			return 3 * 8 + stoi(operand.substr(1, 1));
		}
		else {
			// mem
			return 2 * 8 + 0;
		}
	}

	// paramsBefore sluzi da znamo za koju poziciju se pravi realokacija, 
	// govori koliko parametara ima pre trenutnog u direktivi 
	// npr .char 12, 34, a, 12+3*(7-1)
	// za a ce se makeRealloc pozvati sa vrednoscu paramsBefore = 2
	void makeRealloc(Symbol& symbol, bool pcrel, int paramsBefore = 0) {
		if (locationOffset == -1) {
			// glupav hack
			// ako je instrukcija kreirana "vestacki" (tj. iz jmp ili ret instrukcije)
			// onda ce imati locationOffset=-1 (po default-u)
			return;
		}
		if (pcrel) {
			if (symbol.section == section) {
				// ne pravi se relokacioni zapis
				// pomeraj = offset(symbol) - adresa sledece instrukcije
				//  to je adresa trenutne instrukcije + size - 2
				// pcrel je sigurno za instrukciju, i to za onu od 4 bajta, pa je size 4
				int diff = symbol.offset - (locationOffset + 4);
				// treba obrnuti 2 bajta
				string hex = toHex(diff, 2);
				availableSections[section].content += hex.substr(2);
				availableSections[section].content += hex.substr(0, 2);
			}
			else {
				if (symbol.visibility == 'g') {
					string hex = toHex(-2, 2);
					availableSections[section].content += hex.substr(2);
					availableSections[section].content += hex.substr(0, 2);
					Realloc r = Realloc(locationOffset + 2, reallocationTypes[3], symbol.entryNumber, section);
					allReallocations.push_back(r);
				}
				else {
					// lokalni iz druge sekcije
					string hex = toHex(symbol.offset - 2, 2);
					availableSections[section].content += hex.substr(2);
					availableSections[section].content += hex.substr(0, 2);
					Symbol fromSection;
					string tmp = "." + symbol.section;
					for (Symbol var : symbolTable)
					{
						if (var.name == tmp) {
							fromSection = var;
							break;
						}
					}
					// neka pukne ako ne nadje tu sekciju
					Realloc r = Realloc(locationOffset + 2, reallocationTypes[3], fromSection.entryNumber, section);
					allReallocations.push_back(r);
				}
			}
		}
		else {
			if (symbol.section == section) {
				Symbol fromSection;
				string tmp = "." + symbol.section;
				for (Symbol var : symbolTable)
				{
					if (var.name == tmp) {
						fromSection = var;
						break;
					}
				}

				if (directive != "") {
					int a = directive == "char" ? 1 : 4;
					string type = directive == "char" ? reallocationTypes[0] : reallocationTypes[1];

					string hex = toHex(symbol.offset, a);
					for (int i = 0; i < a; i++)
					{
						availableSections[section].content += hex.substr(a * 2 - 2 * (i + 1), 2);
					}

					/*
					ekvivalent
					string b1 = hex.substr(0, 2);
					string b2 = hex.substr(2, 2);
					string b3 = hex.substr(4, 2);
					string b4 = hex.substr(6, 2);
					*/

					Realloc r = Realloc(locationOffset + a*paramsBefore, type, fromSection.entryNumber, section);
					allReallocations.push_back(r);
				}
				else {
					string hex = toHex(symbol.offset, 2);
					availableSections[section].content += hex.substr(2);
					availableSections[section].content += hex.substr(0, 2);
					Realloc r = Realloc(locationOffset + 2, reallocationTypes[2], fromSection.entryNumber, section);
					allReallocations.push_back(r);

				}

			}
			else {
				if (symbol.visibility == 'g') {
					if (directive != "") {
						int a = directive == "char" ? 1 : 4;
						string type = directive == "char" ? reallocationTypes[0] : reallocationTypes[1];

						string hex = toHex(0, a);
						for (int i = 0; i < a; i++)
						{
							availableSections[section].content += hex.substr(a * 2 - 2 * (i + 1), 2);
						}

						Realloc r = Realloc(locationOffset + a*paramsBefore, type, symbol.entryNumber, section);
						allReallocations.push_back(r);
					}
					else {
						availableSections[section].content += toHex(0, 2);
						Realloc r = Realloc(locationOffset + 2, reallocationTypes[2], symbol.entryNumber, section);
						allReallocations.push_back(r);
					}
				}
				else {
					// lokalni iz druge sekcije
					Symbol fromSection;
					string tmp = "." + symbol.section;
					for (Symbol var : symbolTable)
					{
						if (var.name == tmp) {
							fromSection = var;
							break;
						}
					}
					if (directive != "") {
						int a = directive == "char" ? 1 : 4;
						string type = directive == "char" ? reallocationTypes[0] : reallocationTypes[1];

						string hex = toHex(symbol.offset, a);
						for (int i = 0; i < a; i++)
						{
							availableSections[section].content += hex.substr(a * 2 - 2 * (i + 1), 2);
						}

						Realloc r = Realloc(locationOffset + a*paramsBefore, type, fromSection.entryNumber, section);
						allReallocations.push_back(r);
					}
					else {
						string hex = toHex(symbol.offset, 2);
						availableSections[section].content += hex.substr(2);
						availableSections[section].content += hex.substr(0, 2);
						Realloc r = Realloc(locationOffset + 2, reallocationTypes[2], fromSection.entryNumber, section);
						allReallocations.push_back(r);
					}
				}
			}
		}
	}

	void decodeCLW() {
		int args = params.size();
		smatch match;
		for (int i = 0; i < args; i++)
		{
			string param = params[i];
			int a = 0;
			if (regex_search(param, match, regex(R"(^(-?\d+|0x[\da-f]+|0b[01]+)$)"))) {
				a = stoi(ExpressionSolver::toDecimal(param));
			}
			else if (regex_search(param, match, regex(R"(^(".*")$)"))) {
				for (char c : param)
				{
					a += c;
				}
			}
			else if (regex_search(param, match, regex(R"(^('.')$)"))) {
				a = param[0];
			}
			else if (regex_search(param, match, regex(R"(^.?\w+$)"))) {
				// labela
				bool found = false;
				Symbol s;
				for (Symbol s1 : symbolTable)
				{
					if (s1.name == param) {
						found = true;
						a = s1.offset;
						s = s1;
						break;
					}
				}
				if (!found) {
					raiseError("can't evaluate label", lineNumber);
				}
				// napravi relokacioni zapis
				makeRealloc(s, false, i);
				a = std::numeric_limits<int>::min();
			}
			else {
				// expression
				a = ExpressionSolver::parseAndSolveExpression(param);
				if (a == std::numeric_limits<int>::min()) {
					raiseError("can't evaluate expression", lineNumber);
				}

			}
			if (a != std::numeric_limits<int>::min()) {
				if (directive == "char") {
					availableSections[section].content += toHex(a, 1);
				}
				else {
					// long or word
					string hex = toHex(a, 4);
					string b1 = hex.substr(0, 2);
					string b2 = hex.substr(2, 2);
					string b3 = hex.substr(4, 2);
					string b4 = hex.substr(6, 2);
					availableSections[section].content += b4;
					availableSections[section].content += b3;
					availableSections[section].content += b2;
					availableSections[section].content += b1;
				}
			}
		}
	}


	void decodeAlign() {
		int args = params.size();
		int align = 4;
		int fill = 0;
		if (args > 0) {
			align = stoi(params[0]);
		}
		if (args > 1) {
			fill = stoi(ExpressionSolver::toDecimal(params[1]));
		}

		string res = "";
		for (int i = 0; i < align; i++)
		{
			res += toHex(fill, 1);
		}
		availableSections[section].content += res;
	}

	void decodeSkip() {
		int args = params.size();
		int skip = stoi(params[0]);
		int value = 0;
		if (args > 1) {
			value = stoi(ExpressionSolver::toDecimal(params[1]));
		}

		string res = "";
		for (int i = 0; i < skip; i++)
		{
			res += toHex(value, 1);
		}
		availableSections[section].content += res;
	}

	string print() {
		stringstream out;
		string delimiter = "/";
		if (instruction != "") {
			out << lineNumber << ":" << locationOffset << "-" << "INSTRUCTION:" << instruction << delimiter << section << delimiter << dst << delimiter << src << delimiter << condition << "\r\n";
		}
		else {
			out << lineNumber << ":" << locationOffset << "-" << "DIRECTIVE:" << directive << delimiter << section;
			for (string s : params)
			{
				out << delimiter << s;
			}
			out << "\r\n";
		}
		return out.str();
	}


	string stringToLower(string line) {
		string lower = "";
		for (char l : line)
		{
			lower += tolower(l);
		}
		return lower;
	}

};



int checkInstruction(string line) {
	smatch match;

	for (regex inst : instructionsArithmetic)
	{
		if (regex_search(line, match, inst)) {
			// extract dst and src,
			// check dst != immediate addressing,
			// decode which addressing types dst, src are
			// and that not both are relative
			string instruction = match[1];
			string condition = match[2];
			string dst = match[3];
			string src = match[4];
			string dstAdr = adrType(dst);
			string srcAdr = adrType(src);
			if (dstAdr == "" || srcAdr == "") {
				raiseError("invalid addresssing type");
			}
			else if (dstAdr == "imm") {
				raiseError("immediate addressing type is not valid for dst operand");
			}
			else if (dstAdr != "regdir" && srcAdr != "regdir") {
				raiseError("only one operand can require additional 2 bytes");
			}
			if (dstAdr == "regdir" && srcAdr == "regdir") {
				Instruction tmp = Instruction(instruction, condition, dst, src, dstAdr, srcAdr);
				instructions.push_back(tmp);
				return 2;
			}
			else {
				Instruction tmp = Instruction(instruction, condition, dst, src, dstAdr, srcAdr);
				instructions.push_back(tmp);
				return 4;
			}
		}
	}

	if (regex_search(line, match, regexIret)) {
		string instruction = match[1];
		string condition = match[2];
		Instruction tmp = Instruction(instruction, condition);
		instructions.push_back(tmp);
		return 2;
	}
	else if (regex_search(line, match, regexRet)) {
		string instruction = match[1];
		string condition = match[2];
		Instruction tmp = Instruction(instruction, condition);
		instructions.push_back(tmp);
		return 2;
	}
	else if (regex_search(line, match, regexPush)) {
		string instruction = match[1];
		string condition = match[2];
		string src = match[3];
		string srcAdr = adrType(src);
		if (srcAdr == "") {
			raiseError("invalid addresssing type");
		}
		else if (srcAdr == "regdir") {
			Instruction tmp = Instruction(instruction, condition, "", src, "", srcAdr);
			instructions.push_back(tmp);
			return 2;
		}
		else {
			Instruction tmp = Instruction(instruction, condition, "", src, "", srcAdr);
			instructions.push_back(tmp);
			return 4;
		}
	}
	else if (regex_search(line, match, regexPop)) {
		string instruction = match[1];
		string condition = match[2];
		string dst = match[3];
		string dstAdr = adrType(dst);
		if (dstAdr == "") {
			raiseError("invalid addresssing type");
		}
		else if (dstAdr == "imm") {
			raiseError("immediate addressing type is not valid for dst operand");
		}
		else if (dstAdr == "regdir") {
			Instruction tmp = Instruction(instruction, condition, dst, "", dstAdr, "");
			instructions.push_back(tmp);
			return 2;
		}
		else {
			Instruction tmp = Instruction(instruction, condition, dst, "", dstAdr, "");
			instructions.push_back(tmp);
			return 4;
		}
	}
	else if (regex_search(line, match, regexCall)) {
		string instruction = match[1];
		string condition = match[2];
		string src = match[3];
		string srcAdr = adrType(src);
		if (srcAdr == "") {
			raiseError("invalid addresssing type");
		}
		else if (srcAdr == "regdir") {
			Instruction tmp = Instruction(instruction, condition, "", src, "", srcAdr);
			instructions.push_back(tmp);
			return 2;
		}
		else {
			Instruction tmp = Instruction(instruction, condition, "", src, "", srcAdr);
			instructions.push_back(tmp);
			return 4;
		}
	}
	else if (regex_search(line, match, regexJmp)) {
		string instruction = match[1];
		string condition = match[2];
		string dst = match[3];
		string dstAdr = adrType(dst);
		if (dstAdr == "") {
			raiseError("invalid addresssing type");
		}
		else if (dstAdr == "imm") {
			raiseError("immediate addressing type is not valid for dst operand");
		}
		else if (dstAdr == "regdir") {
			Instruction tmp = Instruction(instruction, condition, dst, "", dstAdr, "");
			instructions.push_back(tmp);
			return 2;
		}
		else {
			Instruction tmp = Instruction(instruction, condition, dst, "", dstAdr, "");
			instructions.push_back(tmp);
			return 4;
		}
	}
	else return -1;

}

int checkDirective(string& line) {
	smatch match;
	if (regex_search(line, match, regexChar) && match.size() > 1) {
		string expressions = line.substr(5);
		trim(expressions);
		regex expr(R"(\s*,\s*)");
		smatch matches;
		int count = 1;
		Instruction tmp = Instruction(match[1]);
		while (regex_search(expressions, matches, expr)) {
			tmp.params.push_back(matches.prefix());
			count++;
			expressions = matches.suffix();
		}
		tmp.params.push_back(expressions);
		instructions.push_back(tmp);
		return count;
	}
	else if (regex_search(line, match, regexWord) && match.size() > 1) {
		string expressions = line.substr(5);
		trim(expressions);
		regex expr(R"(\s*,\s*)");
		smatch matches;
		int count = 1;
		Instruction tmp = Instruction(match[1]);
		while (regex_search(expressions, matches, expr)) {
			tmp.params.push_back(matches.prefix());
			count++;
			expressions = matches.suffix();
		}
		tmp.params.push_back(expressions);
		instructions.push_back(tmp);
		return 4 * count;
	}
	else if (regex_search(line, match, regexLong) && match.size() > 1) {
		string expressions = line.substr(5);
		trim(expressions);
		regex expr(R"(\s*,\s*)");
		smatch matches;
		int count = 1;
		Instruction tmp = Instruction(match[1]);
		while (regex_search(expressions, matches, expr)) {
			tmp.params.push_back(matches.prefix());
			count++;
			expressions = matches.suffix();
		}
		tmp.params.push_back(expressions);
		instructions.push_back(tmp);
		return 4 * count;
	}
	else if (regex_search(line, match, regexGlobal)) {
		// len(".global")
		string expressions = line.substr(7);
		regex expr("\\w+");
		smatch matches;
		int count = 0;
		Instruction tmp = Instruction(match[1]);
		while (regex_search(expressions, matches, expr)) {
			// cout << matches[0] << "\r\n";
			tmp.params.push_back(matches[0]);
			count++;
			expressions = matches.suffix();
		}
		instructions.push_back(tmp);
		return 0;
	}
	else if (regex_search(line, match, regexAlign)) {
		if (match[2] == "") {
			// samo full match, znaci bez parametara (samo .align)
			Instruction tmp = Instruction(match[1]);
			// tmp.params ce biti empty?
			instructions.push_back(tmp);
			return 4;
		}
		else if (match[3] == "") {
			int a = stoi(match[2]);
			if (isPow2(a)) {
				Instruction tmp = Instruction(match[1]);
				// bice string, treba kasnije u Instruction.h prebaciti u int!
				tmp.params.push_back(match[2]);
				instructions.push_back(tmp);
				return a;
			}
			else {
				return -1;
			}
		}
		else {
			int a = stoi(match[2]);
			// int b = stoi(match[3]);
			if (isPow2(a)) {
				Instruction tmp = Instruction(match[1]);
				// bice string, treba kasnije u Instruction.h prebaciti u int!
				tmp.params.push_back(match[2]);
				tmp.params.push_back(match[3]);
				instructions.push_back(tmp);
				return a;
			}
			else {
				return -1;
			}
		}
	}
	else if (regex_search(line, match, regexSkip)) {
		Instruction tmp = Instruction(match[1]);
		if (match[3] == "") {
			tmp.params.push_back(match[2]);
			instructions.push_back(tmp);
		}
		else {
			tmp.params.push_back(match[2]);
			tmp.params.push_back(match[3]);
			instructions.push_back(tmp);
		}
		return stoi(match[2]);
	}
	else {
		// not a directive
		return -1;
	}
}

void removeComment(string& line) {
	//regex comment("(;)(.*)");
	int index = line.find_first_of(";");
	line = line.substr(0, index);
}

bool checkLabel(string& line) {
	int index = line.find_first_of(":");
	if (index == -1) return false;
	string label = line.substr(0, index);
	Symbol symbolEntry = Symbol(label, currentSection->name, locationCounter - currentSection->startAddress, 'l', symbolTable.size());
	symbolTable.push_back(symbolEntry);
	line = line.substr(index + 1);
	return true;
}

bool checkSection(string& line) {
	// pretpostavka da u liniji u kojoj je sekcija nema komande
	trim(line);
	string lowerLine = stringToLower(line);
	if (find(validSections.begin(), validSections.end(), lowerLine) != validSections.end()) {
		// remove . at beginning
		currentSection->size = locationCounter - currentSection->startAddress;
		availableSections[currentSection->name] = *currentSection;
		string sectionName = lowerLine.substr(1);
		// ovo sa globalTmp nije bas najsrecnije?
		globalTmp = Section(sectionName, locationCounter);
		currentSection = &globalTmp;
		Symbol symbolEntry = Symbol(lowerLine, sectionName, locationCounter, 'l', symbolTable.size());
		symbolTable.push_back(symbolEntry);
		return true;
	}
	else {
		return false;
	}
}

void firstPass() {
	// sta treba uraditi u prvom prolazu?
	ifstream inputStream;
	inputStream.open(inputFileName);
	if (!inputStream) {
		cout << "Ne moze da se otvori fajl \r\n";
		exit(1);
	}


	Section undefinedSection = Section("undefined", locationCounter);
	currentSection = &undefinedSection;
	Symbol und = Symbol("UND", "undefined", locationCounter, 'l', 0);
	symbolTable.push_back(und);

	// read file
	string line;
	int r;
	smatch match;
	bool isLabel, isSection;

	while (!inputStream.eof()) {
		getline(inputStream, line);
		inputFileLine++;
		removeComment(line);
		trim(line);
		if (line.empty()) {
			continue;
		}
		if (stringToLower(line) == ".end") {
			currentSection->size = locationCounter - currentSection->startAddress;
			availableSections[currentSection->name] = *currentSection;
			break;
		}

		lines.push_back(line);

		// checks if line contains label, if it does - adds to symbolTable
		isLabel = checkLabel(line);
		// same, but for section
		isSection = checkSection(line);

		if (!isSection) {
			if (isLabel == true && line == "") continue;
			if (currentSection == &undefinedSection) {
				if (!regex_search(line, match, regexGlobal)) {
					raiseError("not a .global directive");
				}
				else {
					string expressions = line.substr(7);
					regex expr("\\w+");
					smatch matches;
					int count = 0;
					Instruction tmp = Instruction(match[1]);
					while (regex_search(expressions, matches, expr)) {
						// cout << matches[0] << "\r\n";
						tmp.params.push_back(matches[0]);
						count++;
						expressions = matches.suffix();
					}
					instructions.push_back(tmp);
				}
			}

			else if (currentSection->name != "text") {
				// mogu samo direktive u sekcijama koje nisu .text
				r = checkDirective(line);
				if (r == -1) {
					// greska, prekid programa
					// poruka "error @inputFileLine, not a directive"
					raiseError("not a directive");
				}
				locationCounter += r;
			}
			else {
				// decode how much space the instruction takes and increment lc
				r = checkDirective(line);
				if (r == -1) {
					r = checkInstruction(line);
					if (r == -1) {
						raiseError("not a directive nor instruction");
					}
					locationCounter += r;
				}

			}
		}
	}

	inputStream.close();

}

bool inSymbolTable(string symbolName) {
	bool found = false;
	for (Symbol tableEntry : symbolTable)
	{
		if (symbolName == tableEntry.name) {
			found = true;
			break;
		}
	}
	return found;
}

void printSymbolTable() {
	stringstream out;
	auto delimiter = setw(20);
	out << "#SymbolTable" << "\r\n";
	out << right << "#no" << delimiter << "symbol" << delimiter << "section" << delimiter << "offset" << delimiter << "visibility" << "\r\n";
	for (Symbol symbol : symbolTable)
	{
		out << symbol.entryNumber << delimiter << symbol.name << delimiter << symbol.section << delimiter << toHex(symbol.offset, -1) << delimiter << symbol.visibility << "\r\n";
	}
	outputFileText += out.str();
}

void secondPass() {
	/*
	for (Instruction ins : instructions)
	{
	outputFileText += ins.print();
	}

	outputFileText += "\r\n";
	*/

	locationCounter = startingAddress;
	for (Instruction i : instructions)
	{
		if (i.directive == "global") {
			for (string symbol : i.params)
			{
				bool found = false;
				for (Symbol& tableEntry : symbolTable)
				{
					if (symbol == tableEntry.name) {
						tableEntry.visibility = 'g';
						found = true;
						break;
					}
				}
				if (!found) {
					Symbol symbolEntry = Symbol(symbol, "undefined", 0, 'g', symbolTable.size());
					symbolTable.push_back(symbolEntry);
				}
			}
		}
	}

	for (Instruction instruction : instructions)
	{
		if (instruction.directive != "") {

			if (instruction.directive == "align") {
				instruction.decodeAlign();
			}
			else if (instruction.directive == "skip") {
				instruction.decodeSkip();
			}
			else if (instruction.directive == "char") {
				instruction.decodeCLW();
			}
			else if (instruction.directive == "long") {
				instruction.decodeCLW();
			}
			else if (instruction.directive == "word") {
				instruction.decodeCLW();
			}

		}
		// end if (instruction.directive != "")

		else {
			// decoding instructions
			instruction.decodeInstruction();
		}
	}


	printSymbolTable();
	outputFileText += "\r\n\r\n";
	map<string, Section>::iterator it;
	for (it = availableSections.begin(); it != availableSections.end(); it++)
	{
		if (it->first != "undefined") {
			it->second.printRealloc();
			it->second.printContent();
			outputFileText += "\r\n\r\n";
		}
	}

}

void wait() {
	int a;
	cin >> a;
}

int main(int argc, char* argv[]) {
	// 4 argumenta,
	// argv[0] - naziv programa
	// argv[1] - naziv ulaznog fajla - OBAVEZAN
	// argv[2] - naziv izlaznog fajla (default "OUT"+argv[1])
	// argv[3] - pocetna adresa (default 0)

	if (argc < 2) {
		cout << "Mora se zadati naziv ulaznog fajla \r\n";
		return 0;
	}
	// ako ima vise od 4 argumenta, nema veze, gledamo samo prva 4
	inputFileName = argv[1];

	if (argc < 3) {
		int index = inputFileName.find_last_of("\\");
		if (index > -1) {
			outputFileName = inputFileName.substr(0, index + 1) + "OUT" + inputFileName.substr(index + 1);
		}
		else {
			outputFileName = "OUT" + inputFileName;
		}
	}
	else {
		outputFileName = argv[2];
	}

	if (argc < 4){
		startingAddress = 0;
	}
	else {
		startingAddress = atoi(argv[3]);
	}

	locationCounter = startingAddress;
	outputFileText = "";

	// prvi prolaz
	firstPass();

	// drugi prolaz
	secondPass();


	// ispis u izlazni fajl, zatvaranje fajlova
	ofstream outputStream;
	outputStream.open(outputFileName);
	if (!outputStream) {
		cout << "Ne moze da se otvori fajl \r\n";
		return 0;
	}
	outputStream << outputFileText;
	outputStream.close();

	return 0;
}


