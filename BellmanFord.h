#pragma once
#include "Useful.h"

using namespace std;
class BellmanFord
{
private:
	bool found;
	int size;
	int** matrix_weight;
	int *shortest;
	char** used_paths;
	const int infinity = 2147483647;

public:

	int start;
	BellmanFord()
	{
		found = 0;
		start = -1;
		size = 0;
		matrix_weight = nullptr;
		shortest = nullptr;
		used_paths = nullptr;
	}

	~BellmanFord()
	{
		clear();
	}

	void clear()
	{
		if (size)
		{
			if (matrix_weight)
			{
				for (int i = 0; i < size; i++) free(matrix_weight[i]);
				free(matrix_weight);
			}
			if (used_paths)
			{
				for (int i = 0; i < size; i++) free(used_paths[i]);
				free(used_paths);
			}
			if (shortest) free(shortest);
		}
		size = 0; 
		found = 0;
		start = -1;
	}

	void set_start()
	{
		if (size)
		{
			start = -1;
			while (start < 0 && start >= size)
			{
				cout << "Enter number from 0 to " << size - 1;
				cin >> start;
				if (start < 0 && start >= size) cout << "Invalid arguments";
			}
		}
		else throw runtime_error("Can not be chosen ");
	};

	void find_paths()
	{
		if (size && matrix_weight && start >= 0)
		{
			shortest = (int*)malloc(size * sizeof(int));
			for (int v = 0; v < size; v++)
				shortest[v] = infinity;
			shortest[start] = 0;
			used_paths[start] = "";

			bool changed = 1;
			for (int n = 0; n < size && changed; n++)
			{
				changed = 0;
				for (int i = 0; i < size; i++)
					for (int j = 0; j < size && shortest[i] != infinity; j++)
						if (matrix_weight[i][j] && i != j)	//if node is available and edge exists
							if (shortest[j] == infinity)
							{
								shortest[j] = shortest[i] + matrix_weight[i][j];
								used_paths[j] = string_concat(used_paths[i], string_concat(inttostr(i), ", "));
							}
							else
								if (shortest[j] > shortest[i] + matrix_weight[i][j])
								{
									shortest[j] = shortest[i] + matrix_weight[i][j];
									changed = 1;
									free(used_paths[j]);
									used_paths[j] = string_concat(used_paths[i], string_concat( inttostr(i),", "));
								}
			}
			found = 1;
			output_paths();
			output_shortest();
		}
		else throw runtime_error("Can not be applied to empty matrix");
	};

	void get_matrix_weight()
	{
		ifstream input_file("Text.txt");
		if (input_file.is_open())
		{
			int **matrix = nullptr;
			int *lengths = nullptr;
			size = 0;
			int max_len = 0;

			while (!input_file.eof())
			{
				int in_line = 0;
				matrix = (int**)realloc(matrix, (size + 1) * sizeof(int*));
				lengths = (int*)realloc(lengths, (size + 1) * sizeof(int));

				char line[255];	//may be not enough
				input_file.getline(line, sizeof(line));				//get line

				char** linewords = line_as_words(line, in_line);	//divide the line into "words" on whitespace

				matrix[size] = (int*)malloc(in_line * sizeof(int));
				for (int i = 0; i < in_line; i++)
					matrix[size][i] = strtoint(linewords[i]);		//return words from line as integers

				lengths[size] = in_line;
				if (in_line > max_len) max_len = in_line;
				size++;
			}
			input_file.close();

			for (int i = 0; i < size; i++)				//make square matrix
				if (max_len > lengths[i])
					matrix[i] = (int*)realloc(matrix[i], max_len * sizeof(int));

			if (max_len > size)
			{
				lengths = (int*)realloc(lengths, max_len * sizeof(int));
				for (int i = size; i < max_len; i++)
					lengths[i] = 0;
				matrix = (int**)realloc(matrix, size * sizeof(int*));
				for (int i = size; i < max_len; i++)
					matrix[i] = (int*)malloc(max_len * sizeof(int));
				size = max_len;
			}

			for (int i = 0; i < size; i++)			//add zeros to fill square matrix
				for (int j = size - 1; j >= lengths[i]; j--)
					matrix[i][j] = 0;

			matrix_weight = matrix;

			used_paths = (char**)malloc(size * sizeof(char*));
			for (int i = 0; i < size; i++)
				used_paths[i] = nullptr;
		}
		else matrix_weight = nullptr;
	};

	void output_matrix()
	{
		if (size && matrix_weight)
		{
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++) printf("%4i ", matrix_weight[i][j]);
				cout << endl;
			}
		}
	};

	void output_paths()
	{
		if (found)
		{
			cout << endl;
			for (int i = 0; i < size; i++)
				cout << "node " << i + 1 << ": " << used_paths[i] << endl;
		}
	}

	void output_shortest()
	{
		if (shortest && size)
		{
			for (int i = 0; i < size; i++)
				cout << shortest[i] << " ";
		}
	};
};
