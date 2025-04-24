#ifndef TESTER_HPP
#define TESTER_HPP

#include "Timer.hpp"
#include <omp.h>
#include <string>
#include <iostream>
#include <fstream>

#ifndef _OPENMP
#define omp_set_num_threads(threads) 0
#endif

namespace parallel_cgp {
	class Tester
	{
	private:
		std::ofstream myFile;
		std::string testerName;
		std::string filename;
	public:
		const static int ROUNDS = 10;
		const static int GENERATIONS = 1000;
		const static int SMALL_ROWS = 4;
		const static int MEDIUM_ROWS = 8;
		const static int LARGE_ROWS = 10;
		const static int SMALL_COLUMNS = 4;
		const static int MEDIUM_COLUMNS = 8;
		const static int LARGE_COLUMNS = 10;
		const static int SMALL_LEVELS = 0;
		const static int MEDIUM_LEVELS = 1;
		const static int LARGE_LEVELS = 3;
		const static int SMALL_POP_SIZE = 5;
		const static int MEDIUM_POP_SIZE = 8;
		const static int LARGE_POP_SIZE = 16;
		inline const static int threadNums[6] = { 1, 2, 4, 8, 16, 32 };

		Tester(std::string testerName) : testerName(testerName), filename(testerName) {
			filename.append(".csv");
			myFile.open(filename);
			myFile.close();
		}

		void saveResults(std::string testName, int gens, int rows, int cols, int levels, int pop) {
			Timer::saveTimes(filename, testName, gens, rows, cols, levels, pop);

			std::cout << "--------------------------------------" << std::endl;
			std::cout << "TEST NAME: " << testName << std::endl;
			std::cout << "--------------------------------------" << std::endl;
			std::cout << "GENS: " << gens << ", ROWS: " << rows << ", COLUMNS: " << cols
				<< ", LEVELS BACK: " << levels << ", POP SIZE: " << pop << std::endl;
			std::cout << "--------------------------------------" << std::endl;
			Timer::clearTimes();
		}
	};
}

#endif
