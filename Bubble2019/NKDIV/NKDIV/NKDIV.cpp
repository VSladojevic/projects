
#include"pch.h"
#include<iostream>
#include<vector>
#include<string>
#include<Bits.h> 

using namespace std;

// If prime factorization of a number is 2^3 * 3^4 * 5^1, our piles are 3,4,1
 // function to calculate all the prime factors and 
// count of every prime factor 
vector<int> factorize(long long n)
{

	vector<int> piles;
	int count = 0;

	// count the number of times 2 divides 
	while (!(n % 2)) {
		n >>= 1; // equivalent to n=n/2; 
		count++;
	}

	// if 2 divides it 
	if (count) {
		piles.push_back(count);
	}

	// check for all the possible numbers that can 
	// divide it 
	for (long long i = 3; i <= sqrt(n); i += 2) {
		count = 0;
		while (n % i == 0) {
			count++;
			n = n / i;
		}
		if (count)
		{
			piles.push_back(count);
		}
	}

	// if n at the end is a prime number. 
	if (n > 2)
	{
		piles.push_back(1);
	}

	return piles;
}

int gameWinner(vector<int> heaps, int k) {
	if (heaps.size() <= k) return 0;
	// returns 0 if 1st player is the winner,
	// or returns 1 if 2nd player has won

	// calculate nim-sum
	// it is actually an array, for each digit in binary 
	// representation we sum up digits on that position
	// from all numbers(heaps) and calculate modulo (k+1)
	// if all elements of that calculated array are 0, the winner is 2nd player
	int max = 0;
	for (int i = 0; i < heaps.size(); i++)
	{
		if (heaps[i] > max) {
			max = heaps[i];
		}
	}
	vector<int> sums;
	int cur;
	int ind = 0;
	int t;
	while (max > 0) {
		cur = 0;
		t = 1;
		for (int i = 0; i < ind; i++)
		{
			t *= 2;
		}
		for (int i = 0; i < heaps.size(); i++)
		{
			cur += (heaps[i] & t) / t;
		}
		cur = cur % (k + 1);
		sums.push_back(cur);
		max /= 2;
		ind++;
	}
	for (int i = 0; i < sums.size(); i++)
	{
		if (sums[i] != 0) return 0;
	}
	return 1;
}

int main()
{
	int rounds;
	cin >> rounds;
	vector<long long> numbers;
	vector<int> k;
	long long nn;
	int kk;
	for (int i = 0; i < rounds; i++)
	{
		cin >> nn >> kk;
		numbers.push_back(nn);
		k.push_back(kk);
	}



	vector<vector<int> > allPiles;
	for (int i = 0; i < numbers.size(); i++)
	{
		allPiles.push_back(factorize(numbers[i]));
	}

	vector<string> results;
	string player[] = { "Mona","Alaa" };
	int first = 0;
	int winner;
	int count[] = { 0,0 };
	for (int i = 0; i < rounds; i++)
	{
		winner = (first + gameWinner(allPiles[i], k[i])) % 2;
		results.push_back(player[winner]);
		first = winner;
		count[winner]++;
	}

	for (int i = 0; i < results.size(); i++)
	{
		cout << results[i] << endl;
	}
	cout << endl;
	if (count[1] > count[0]) cout << player[1];
	else cout << player[0];

}