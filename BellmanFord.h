#include "stdafx.h"
#include "CppUnitTest.h"
#include "../aisd_lab4_4/BellmanFord.h"
#include "../aisd_lab4_4/Useful.h"

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
			map.set_start(0);
			map.find_paths();

			Assert::AreEqual(12, map.shortest[8]);
		}

		TEST_METHOD(path_4_to_7)
		{
			BellmanFord map;
			map.get_matrix_weight_file();
			map.set_start(4);
			map.find_paths();

			Assert::AreEqual(11, map.shortest[7]);
		}

		TEST_METHOD(path_5_to_6)
		{
			BellmanFord map;
			map.get_matrix_weight_file();
			map.set_start(0);
			map.find_paths();

			Assert::AreEqual(9, map.shortest[6]);
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
				Assert::AreEqual(e.what(), "Start must be from 0 to size-1");
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
	};
}
