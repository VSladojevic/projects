// Frog.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include"pch.h"
#include<iostream>
using namespace std;


void main() {
	int n;
	cin >> n;
	int* a;
	a = new int[n];
	int m = 0;
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
		if (a[i] > m) {
			m = a[i];
		}
	}

	double* ev;
	ev = new double[m + 1];
	ev[1] = 1;
	double s = 0;
	for (int i = 2; i <= m; i++)
	{
		if (i <= 10) {
			s = 0;
			for (int j = 1; j < i; j++)
			{
				s += ev[j];
			}
			ev[i] = 1 + s / i;
		}
		else {
			s = 0;
			for (int j = 1; j <= 10; j++)
			{
				s += ev[i-j];
			}
			ev[i] = 1 + s / 10;
		}
	}

	for (int i = 0; i < n; i++)
	{
		printf("%lf\n", ev[a[i]]);
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
