// GuessTheNumber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

int main()
{
	int testCases;
	cin >> testCases;
	vector<int> N, M;
	int n, w, m;
	for (int i = 0; i < testCases; i++)
	{
		cin >> n >> w >> m;
		N.push_back(n);
		M.push_back(m);
	}
	int r;
	int binR;
	string out;
	char bin[] = { '0','1' };
	for (int i = 0; i < testCases; i++)
	{
		r = rand() % M[i] + 1;
		cout << r << endl;
		for (int j = 0; j < r; j++)
		{
			for (int k = 0; k < N[i]; k++)
			{
				// 0 or 1
				binR = rand() % 2;
				out += bin[binR];
			}
			cout << out << endl;
			out = "";
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
