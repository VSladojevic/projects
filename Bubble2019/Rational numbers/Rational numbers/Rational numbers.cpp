// Rational numbers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include<vector>
#include<string>

using namespace std;

int gcd(int a, int b)
{
	if (b == 0) return a;
	a %= b;
	return gcd(b, a);
}

vector<int> coprimesOfNumber(int num) {
	vector<int> con;
	for (int i = 1; i < num; i++)
	{
		if (gcd(num, i) == 1) con.push_back(i);
	}
	return con;
}

/*
	i is row, j column
	example: 9 corresponds to i = 3, j = 2
*/
string findInRow(int i, int j) {
	if (i == 1) {
		int t = j / 2;
		if (j % 2 == 1) t = -t;
		return to_string(t);
	}

	string s = "";
	vector<int> coprimes = coprimesOfNumber(i);
	int l = coprimes.size();
	if (j % 2 == 0) s = "-";
	j = ceil(j / (double)2);
	j = j - 1;
	int val = j / l;
	int rem = j % l;
	int res = coprimes[rem] + val * i;
	s = s + to_string(res);
	s = s + "/";
	s = s + to_string(i);
	return s;
}

int main()
{
	int n;
	cin >> n;
	int* a;
	a = new int[n];
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	int i = 0, j = 0;
	int m = 0, k = 0;
	int num;
	double d;
	for (int ii = 0; ii < n; ii++)
	{
		num = a[ii];
		d = (sqrt(8 * num + 1) - 3) / 2;
		m = ceil(d);
		k = num - m * (m + 1) / 2;
		i = k;
		j = m + 1 - (k - 1);
		if (num == 1) {
			i = 1;
			j = 1;
		}
		//cout << i << ", " << j << endl;
		cout << findInRow(i, j) << endl;
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
