#pragma once
#include <vector>
#include <iostream>

using std::vector;

template <unsigned int SIZE_X, unsigned int SIZE_Y>
class Picross_Board
{
private:
	vector<unsigned int> row_data[SIZE_Y];
	vector<unsigned int> col_data[SIZE_X];
	int data[SIZE_Y][SIZE_X];

	vector<vector<unsigned int>> row_sample[SIZE_Y];
	vector<vector<unsigned int>> col_sample[SIZE_X];

public:
	Picross_Board(const vector<unsigned int> row_data[], const vector<unsigned int> col_data[])
		:data{ 0, }
	{
		for (unsigned int i = 0; i < SIZE_Y; i++)
			this->row_data[i] = row_data[i];
		for (unsigned int i = 0; i < SIZE_X; i++)
			this->col_data[i] = col_data[i];

		if (!isValidData())
		{
			perror("Failed To Setting Picross_Board Data");

			for (unsigned int i = 0; i < SIZE_Y; i++)
				this->row_data[i].clear();
			for (unsigned int i = 0; i < SIZE_X; i++)
				this->col_data[i].clear();

			return;
		}
	}



	bool isValidRowData() const
	{
		for (unsigned int i = 0; i < SIZE_Y; i++)
		{
			int cnt = -1;

			if (row_data[i].size() == 0)
				continue;

			for (unsigned int ui : row_data[i])
			{
				if (ui == 0)
					return false;

				cnt += ui + 1;
			}

			if (cnt < 0 || cnt > SIZE_X)
				return false;
		}

		return true;
	}

	bool isValidColData() const
	{
		for (unsigned int i = 0; i < SIZE_X; i++)
		{
			int cnt = -1;

			if (col_data[i].size() == 0)
				continue;

			for (unsigned int ui : col_data[i])
			{
				if (ui == 0)
					return false;

				cnt += ui + 1;
			}

			if (cnt < 0 || cnt > SIZE_Y)
				return false;
		}

		return true;
	}

	bool isValidData() const
	{
		if (!isValidRowData() || !isValidColData())
			return false;

		int cnt = 0;

		for (const vector<unsigned int> &vui : row_data)
			for (unsigned int ui : vui)
				cnt += ui;

		for (const vector<unsigned int>& vui : col_data)
			for (unsigned int ui : vui)
				cnt -= ui;
		
		if (cnt != 0)
		{
			std::cout << "Balance : " << cnt << std::endl;
			return false;
		}

		return true;
	}

	void makeSample()
	{
		for (unsigned int i = 0; i < SIZE_Y; i++)
			_makeSample(row_sample[i], row_data[i], SIZE_X);

		for (unsigned int i = 0; i < SIZE_X; i++)
			_makeSample(col_sample[i], col_data[i], SIZE_Y);
	}

	int solve(void)
	{
		for (unsigned int i = 0; i < SIZE_Y; i++)
		{
			int sum[SIZE_X] = { 0, };
			int temp[SIZE_X];
			bool isDelete;

			if (row_data[i].size() && row_sample[i].size() == 0)
				return -1;
			
			for (unsigned int j = 0; j < row_sample[i].size(); j++)
			{
				for (int& t : temp)
					t = 2;

				for (unsigned int k = 0; k < row_sample[i][j].size(); k++)
				{
					for (unsigned int l = 0; l < row_data[i][k]; l++)
					{
						temp[row_sample[i][j][k] + l] = 1;
					}
				}

				isDelete = false;
				// Remove
				for (unsigned int k = 0; k < SIZE_X; k++)
				{
					if (data[i][k] & ~temp[k])
					{
						isDelete = true;
						break;
					}
				}

				// Add
				if (isDelete)
				{
					row_sample[i].erase(row_sample[i].begin() + j);
					j--;
					continue;
				}

				for (unsigned int k = 0; k < SIZE_X; k++)
				{
					sum[k] |= temp[k];
				}
			}

			for (int& s : sum)
				if (s == 0)
					s = 2;

			for (unsigned int j = 0; j < SIZE_X; j++)
				data[i][j] = (sum[j] == 3) ? 0 : sum[j];
		}

		for (unsigned int i = 0; i < SIZE_X; i++)
		{
			int sum[SIZE_Y] = { 0, };
			int temp[SIZE_Y];
			bool isDelete;

			if (col_data[i].size() && col_sample[i].size() == 0)
				return -1;

			for (unsigned int j = 0; j < col_sample[i].size(); j++)
			{
				for (int& t : temp)
					t = 2;

				for (unsigned int k = 0; k < col_sample[i][j].size(); k++)
				{
					for (unsigned int l = 0; l < col_data[i][k]; l++)
					{
						temp[col_sample[i][j][k] + l] = 1;
					}
				}

				isDelete = false;
				for (unsigned int k = 0; k < SIZE_Y; k++)
				{
					if (data[k][i] & ~temp[k])
					{
						isDelete = true;
						break;
					}
				}

				if (isDelete)
				{
					col_sample[i].erase(col_sample[i].begin() + j);
					j--;
					continue;
				}

				for (unsigned int k = 0; k < SIZE_Y; k++)
				{
					sum[k] |= temp[k];
				}
			}

			for (int& s : sum)
				if (s == 0)
					s = 2;

			for (unsigned int j = 0; j < SIZE_Y; j++)
				data[j][i] = (sum[j] == 3) ? 0 : sum[j];
		}

		return 0;
	}

	static void _makeSample(vector<vector<unsigned int>>& output, vector<unsigned int> data, unsigned int size)
	{
		vector<unsigned int> iter(data.size());
		output.clear();

		if (data.size() == 0)
			return;

		for (iter[0] = 0; iter[0] + data[0] <= size && __makeSample(output, data, iter, 1, size); iter[0]++);
	}


private:
	static bool __makeSample(vector<vector<unsigned int>>& output, vector<unsigned int> data, vector<unsigned int>& iter, int ind, unsigned int size)
	{
		if (ind == data.size())
		{
			output.push_back(iter);
			return true;
		}

		for (iter[ind] = iter[ind - 1] + data[ind - 1] + 1; iter[ind] + data[ind] <= size && __makeSample(output, data, iter, ind + 1, size); iter[ind]++);

		return true;
	}



public:
	void showBoard(void) const
	{
		for (unsigned int i = 0; i < SIZE_Y; i++)
		{
			for (unsigned int j = 0; j < SIZE_X; j++)
			{
				switch (data[i][j])
				{
				case  0:std::cout << "¡¡"; break;
				case  1:std::cout << "¡á"; break;
				case  2:std::cout << "£ø"; break;
				default:std::cout << "ER"; break;
				}

				std::cout << ((j % 5 == 4) ? "|" : " ");
			}
			
			std::cout << "¤Ó";
			for (unsigned int ui : row_data[i])
				std::cout << ((ui % 100 < 10) ? "  " : " ") << (ui % 100);
			std::cout << std::endl;

			if (i % 5 == 4)
				std::cout << std::endl;
		}

		for (unsigned int i = 0; i < SIZE_X; i++)
			std::cout << "¤Ñ" << ((i % 5 == 4) ? "|" : " ");
		std::cout << std::endl;
		
		bool loop_check = true;
		for (unsigned int i = 0; loop_check; i++)
		{
			loop_check = false;
			for (unsigned int j = 0; j < SIZE_X; j++)
			{
				if (i < col_data[j].size())
				{
					std::cout << ((col_data[j][i] % 100 < 10) ? " " : "") << (col_data[j][i] % 100) << ((j % 5 == 4) ? "|" : " ");
					loop_check = true;
				}
				else
					std::cout << "  "<< ((j % 5 == 4) ? "|" : " ");
			}
			std::cout << std::endl;
		}
	}
};

