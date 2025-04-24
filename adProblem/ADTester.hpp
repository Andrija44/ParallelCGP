#ifndef ADTESTER_HPP
#define ADTESTER_HPP

#include "../Tester.hpp"
#include "../Timer.hpp"
#include "ADProblem.hpp"

namespace parallel_cgp {
	struct ADParam {
		ADParam() {}
		ADParam(int gens, int rows, int cols, int levels, int pop) : gens(gens), rows(rows), cols(cols), levels(levels), pop(pop) {}
		int gens;
		int rows;
		int cols;
		int levels;
		int pop;
	};

	class SeqADTester : private Tester
	{
	private:
		std::string funcs[3] = { "smallSeqADTest", "mediumSeqADTest", "largeSeqADTest" };
		ADParam params[3] = { ADParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			ADParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			ADParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE) {
			Timer testTimer("adTestTimer");

			ADProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		SeqADTester() : Tester("SeqADTest") {
			for (int f = 0; f < (sizeof(funcs) / sizeof(*funcs)); f++) {
				for (int i = 0; i < ROUNDS; i++) {
					test(funcs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop);
				}
			}
		}
	};

	class ParADTester : private Tester
	{
	private:
		std::string funcs[3] = { "smallParADTest", "mediumParADTest", "largeParADTest" };
		ADParam params[3] = { ADParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			ADParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			ADParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int THREAD_NUM) {
			Timer testTimer("adTestTimer");

			omp_set_num_threads(THREAD_NUM);

			ADProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		ParADTester() : Tester("ParADTest") {

			for (int f = 0; f < (sizeof(funcs) / sizeof(*funcs)); f++) {
				for (int t = 0; t < (sizeof(threadNums) / sizeof(*threadNums)); t++) {
					for (int i = 0; i < ROUNDS; i++) {
						test(funcs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, threadNums[t]);
					}
				}
			}
		}
	};
}

#endif
