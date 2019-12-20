#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Picross_Board.h"

#define SIZE_X 25
#define SIZE_Y 25
#define LOCATION "data.txt"

#include <conio.h>

using namespace std;

void readData(vector<unsigned int>* row_data, unsigned int size_y, vector<unsigned int>* col_data, unsigned int size_x)
{
	ifstream fin(LOCATION);
	string temps;
	unsigned int tempui;

	for (unsigned int i = 0; i < size_y; i++)
	{
		getline(fin, temps);
		stringstream ss(temps);

		while (ss >> tempui)
			row_data[i].push_back(tempui);
	}

	getline(fin, temps);

	for (unsigned int i = 0; i < size_x; i++)
	{
		getline(fin, temps);
		stringstream ss(temps);

		while (ss >> tempui)
			col_data[i].push_back(tempui);
	}
}

int main(void)
{
	vector<unsigned int> row[SIZE_Y];
	vector<unsigned int> col[SIZE_X];

	readData(row, SIZE_Y, col, SIZE_X);

	Picross_Board<SIZE_X, SIZE_Y> p(row, col);
	p.makeSample();
	p.showBoard();
	for (unsigned int i = 0; i < 10; i++)
	{
		cout << "i : " << i << endl;
		if (p.solve() == -1)
		{
			perror("해결 불과 문제.");
			return -1;
		}
	}
	p.showBoard();

	return 0;
}