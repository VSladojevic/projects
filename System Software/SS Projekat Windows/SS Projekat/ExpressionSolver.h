#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stack>
#include <map>

using namespace std;


regex operands(R"([^()+\*\/-]+)");

void trim2(string& line) {
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

string toDecimal(string match) {
	stringstream res;
	trim2(match);
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
	// ako je labela treba staviti vrednost labele
	// ako nije, onda proveriti da li su samo cifre
	else res << match;
	return res.str();
}

string prepareExpression(string expression) {
	/* 
	   Zameni labele, binarne i heksadecimalne brojeve dekadnim,
	   a whitespace-ove ukloni.
	*/ 
	stringstream out;
	string s = expression;
	s = regex_replace(s, regex("\\(-"), "(0-");
	s = regex_replace(s, regex("^-"), "0-");

	smatch matches;
	while (regex_search(s, matches, operands)) {
		out << s.substr(0, matches.position(0));
		/*if (regex_search(matches[0], m, regex R("(\w+)"))) {

		}*/
		out << toDecimal(matches[0]);
		s = matches.suffix();
	}

	// !!!
	out << s;

	return out.str();
}

string toPostfix(string expression) {
	stringstream postfix;
	stack<char> opStack;
	string delimiter = " ";
	map<char, int> priorities = { { '+', 2 }, { '-', 2 }, { '*', 3 }, { '/', 3 }, { '(', 1 } };
	for each (char c in expression)
	{
		if (c >= '0' && c <= '9') {
			postfix << c;
		}
		else if(c=='(') {
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

int parseAndSolveExpression(string expression) {
	// kada budem radio second pass, tamo uraditi prepareExpression ako je moguce,
	// pa onda pozvati toPostfix i resavanje
	stack<int> operands;
	vector<string> tokens;
	stringstream out;
	string prepared = prepareExpression(expression);
	string postfix = toPostfix(prepared);
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
