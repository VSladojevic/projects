// Input.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<string>

using namespace std;

struct Tile {
	int botLeftX, botLeftY;
	int topRightX, topRightY;
	Tile(int blX = 0, int blY = 0, int trX = 0, int trY = 0) {
		botLeftX = blX;
		botLeftY = blY;
		topRightX = trX;
		topRightY = trY;
	}
	int area() {
		return (topRightX - botLeftX)*(topRightY - botLeftY);
	}
};

bool tilesOverlap(Tile a, Tile b) {
	if (b.botLeftX >= a.topRightX) return false;
	if (b.topRightX <= a.botLeftX) return false;
	if (b.botLeftY >= a.topRightY) return false;
	if (b.topRightY <= a.botLeftY) return false;
	return true;
}

bool checkOverlap(vector<Tile> tiles, Tile t) {
	if (tiles.size() < 1) return false;
	Tile tmp;
	for (int i = 0; i < tiles.size(); i++)
	{
		tmp = tiles[i];
		if (tilesOverlap(tmp, t)) {
			return true;
		}
	}
	return false;
}

int main()
{
	int floors;
	cin >> floors;
	vector<string> results;
	for (int i = 0; i < floors; i++)
	{
		int area = 0;
		bool nonDisjoint = false;
		bool nonContained = false;
		bool nonCovering = false;
		int w, h;
		cin >> w >> h;
		int noTiles;
		cin >> noTiles;
		vector<Tile> tiles;
		int j = 0;
		for (j = 0; j < noTiles; j++)
		{
			int blX, blY, trX, trY;
			cin >> blX >> blY >> trX >> trY;
			Tile t = Tile(blX, blY, trX, trY);
			area += t.area();
			if (!nonDisjoint) {
				if (checkOverlap(tiles, t)) {
					nonDisjoint = true;
				}
				else {
					tiles.push_back(t);
				}
			}

		}

		if (!nonDisjoint) {
			for (int i = 0; i < noTiles; i++)
			{
				if (tiles[i].botLeftX<0 || tiles[i].topRightX>w || tiles[i].botLeftY<0 || tiles[i].topRightY>h) {
					nonContained = true;
					break;
				}
			}
		}

		if (!nonDisjoint && !nonContained) {
			if (area < w*h) nonCovering = true;
		}

		if (nonDisjoint) results.push_back("NONDISJOINT");
		else if (nonContained) results.push_back("NONCONTAINED");
		else if (nonCovering) results.push_back("NONCOVERING");
		else results.push_back("OK");
		
		
	}

	for (int i = 0; i < results.size(); i++)
	{
		cout << results[i] << endl;
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
