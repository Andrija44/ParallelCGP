#ifndef WAITTESTER_HPP
#define WAITTESTER_HPP

#include "../Tester.hpp"
#include "../Timer.hpp"
#include "WaitProblem.hpp"

namespace parallel_cgp {
	struct WaitParam {
		WaitParam() {}
		WaitParam(int gens, int rows, int cols, int levels, int pop, int time) : gens(gens), rows(rows), cols(cols), levels(levels), pop(pop), time(time) {}
		int gens;
		int rows;
		int cols;
		int levels;
		int pop;
		int time;
	};

	class SeqWaitTester : private Tester
	{
	private:
		std::string funcs[3] = { "smallSeqWaitTest", "mediumSeqWaitTest", "largeSeqWaitTest" };
		WaitParam params[3] = { WaitParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE, 1),
			WaitParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE, 1),
			WaitParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE, 1) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int WAIT_TIME) {
			Timer testTimer("waitTestTimer");

			WaitProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE, WAIT_TIME);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		SeqWaitTester() : Tester("SeqWaitTest") {
			for (int f = 0; f < (sizeof(funcs) / sizeof(*funcs)); f++) {
				for (int i = 0; i < ROUNDS; i++) {
					test(funcs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, params[f].time);
				}
			}
		}
	};

	class ParWaitTester : private Tester
	{
	private:
		std::string funcs[3] = { "smallParWaitTest", "mediumParWaitTest", "largeParWaitTest" };
		WaitParam params[3] = { WaitParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE, 1),
			WaitParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE, 1),
			WaitParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE, 1) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int WAIT_TIME, int THREAD_NUM) {
			Timer testTimer("waitTestTimer");

			omp_set_num_threads(THREAD_NUM);

			WaitProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE, WAIT_TIME);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		ParWaitTester() : Tester("ParWaitTest") {

			for (int f = 0; f < (sizeof(funcs) / sizeof(*funcs)); f++) {
				for (int t = 0; t < (sizeof(threadNums) / sizeof(*threadNums)); t++) {
					for (int i = 0; i < ROUNDS; i++) {
						test(funcs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, params[f].time, threadNums[t]);
					}
				}
			}
		}
	};
}

#endif