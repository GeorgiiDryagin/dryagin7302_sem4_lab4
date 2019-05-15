#include "stdafx.h"
#include "CppUnitTest.h"
#include "../aisd_lab4_4/Useful.h"


using namespace std;
class BellmanFord
{
private:
	bool found;
	int start;
	int size;
	int** matrix_weight;
	char** used_paths;
	const int infinity = 2147483647;

public:
	int *shortest;

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

				for (int i = 0; i < size; i++)
					if (matrix_weight[i]) free(matrix_weight[i]);
				free(matrix_weight);
			}
			if (used_paths)
			{
				for (int i = 0; i < size; i++)
					if (used_paths[i])  free(used_paths[i]);
				free(used_paths);
			}
			if (shortest) free(shortest);
		}
		size = 0;
		found = 0;
		start = -1;
	}

	void set_start(int new_start)
	{
		if (size)
		{
			if (new_start > -1 && new_start < size) start = new_start;
			else throw invalid_argument("Start must be from 0 to size-1");
		}
		else throw runtime_error("Can not be chosen in enmpty matrix");
	};

	void find_paths()
	{
		if (size && matrix_weight && start >= 0)
		{
			shortest = (int*)malloc(size * sizeof(int));
			for (int v = 0; v < size; v++)
				shortest[v] = infinity;
			shortest[start] = 0;
			used_paths[start] = string_copy("");

			for (int n = 0; n < size; n++)
			{
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
									free(used_paths[j]);
									used_paths[j] = string_concat(used_paths[i], string_concat(inttostr(i), ", "));
								}
			}
			found = 1;
		}
		else throw runtime_error("Can not be applied to empty matrix");
	};

	void matrix_generator(int size_r, int max)
	{
		size = size_r;
		matrix_weight = (int**)malloc(size * sizeof(int*));
		for (int i = 0; i < size; i++)
		{
			matrix_weight[i] = (int*)malloc(size * sizeof(int));
			for (int j = 0; j < size; j++)
			{
				if (rand() % 2)
					matrix_weight[i][j] = rand() % max;
				else
					matrix_weight[i][j] = 0;
				if (i == j) matrix_weight[i][j] = 0;
			}
		}

		used_paths = (char**)malloc(size * sizeof(char*));
		for (int i = 0; i < size; i++)
			used_paths[i] = nullptr;
	};

	void get_matrix_weight_file()
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

			for (int i = 0; i < size; i++)				//make matrix square 
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


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(path_0_to_8)
		{
			BellmanFord map;
			map.get_matrix_weight_file();
			try
			{
				map.set_start(0);
				map.find_paths();
				Assert::AreEqual(12, map.shortest[8]);
			}
			catch (runtime_error e)
			{
				//Assert::AreEqual(e.what(), "Can not be applied to empty matrix");
				Assert::AreEqual(e.what(), "Can not be chosen in enmpty matrix");
			}

			//
		}

		TEST_METHOD(path_4_to_7)
		{
			BellmanFord map;
			map.get_matrix_weight_file();
			try
			{
				map.set_start(4);
				map.find_paths();
				Assert::AreEqual(11, map.shortest[7]);
			}
			catch (runtime_error e)
			{
				//Assert::AreEqual(e.what(), "Can not be applied to empty matrix");
				Assert::AreEqual(e.what(), "Can not be chosen in enmpty matrix");
			}

		}

		TEST_METHOD(path_5_to_6)
		{
			BellmanFord map;
			map.get_matrix_weight_file();
			try
			{
				map.set_start(0);
				map.find_paths();
				Assert::AreEqual(9, map.shortest[6]);
			}
			/*catch (invalid_argument e)
			{
			Assert::AreEqual(e.what(), "Start must be from 0 to size - 1");
			}*/
			catch (runtime_error e)
			{
				//Assert::AreEqual(e.what(), "Can not be applied to empty matrix");
				Assert::AreEqual(e.what(), "Can not be chosen in enmpty matrix");
			}
			//
		}

		TEST_METHOD(wrong_start)
		{
			BellmanFord map;
			map.get_matrix_weight_file();
			try
			{
				map.set_start(100);
			}
			catch (invalid_argument e)
			{
				Assert::AreEqual(e.what(), "Start must be from 0 to size - 1");
			}
			catch (runtime_error e)
			{
				Assert::AreEqual(e.what(), "Can not be chosen in enmpty matrix");
			}
		}

		TEST_METHOD(wrong_start_empty)
		{
			BellmanFord map;
			try
			{
				map.set_start(10);
			}
			catch (runtime_error e)
			{
				Assert::AreEqual(e.what(), "Can not be chosen in enmpty matrix");
			}
		}
		
		TEST_METHOD(nothng)
		{

		}


	};
}

