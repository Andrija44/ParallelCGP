#ifndef BOOLTESTER_HPP
#define BOOLTESTER_HPP

#include "../Tester.hpp"
#include "../Timer.hpp"
#include "BoolProblem.hpp"

namespace parallel_cgp {
	struct BoolParam {
		BoolParam() {}
		BoolParam(int gens, int rows, int cols, int levels, int pop) : gens(gens), rows(rows), cols(cols), levels(levels), pop(pop) {}
		int gens;
		int rows;
		int cols;
		int levels;
		int pop;
	};

	class SeqBoolTester : private Tester, private BoolProblem
	{
	private:
		std::string boolFuncs[6] = { "smallSimpleSeqBoolTest", "mediumSimpleSeqBoolTest", "largeSimpleSeqBoolTest", "smallComplexSeqBoolTest", "mediumComplexSeqBoolTest", "largeComplexSeqBoolTest" };
		BoolParam params[6] = { BoolParam(Tester::GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(Tester::GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			BoolParam(Tester::GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(Tester::GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE) };
		std::function<int(std::bitset<INPUTS> in)> func[2] = { [](std::bitset<INPUTS> in) { return (in[0] | ~in[1]) & ((in[0] ^ in[4]) | (in[3] & ~in[2])); }, [](std::bitset<INPUTS> in) { return (((in[0] & ~in[1]) | (in[2] ^ in[3])) & ((in[4] | in[5]) & (~in[6] | (in[0] & in[1])))) | (((in[2] & in[3]) | (in[4] ^ in[5])) & ((in[6] | ~in[0]) & (in[1] | in[2]))); } };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, std::function<int(std::bitset<INPUTS> in)> boolFunc) {
			Timer testTimer("boolTestTimer");

			BoolProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE, boolFunc);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		SeqBoolTester() : Tester("SeqBoolTest") {
			for (int f = 0; f < (sizeof(boolFuncs) / sizeof(*boolFuncs)); f++) {
				for (int i = 0; i < ROUNDS; i++) {
					if (f < 3)
						test(boolFuncs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, func[0]);
					else
						test(boolFuncs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, func[1]);
				}
			}
		}
	};

	class ParBoolTester : private Tester, private BoolProblem
	{
	private:
		std::string boolFuncs[6] = { "smallSimpleParBoolTest", "mediumSimpleParBoolTest", "largeSimpleParBoolTest", "smallComplexParBoolTest", "mediumComplexParBoolTest", "largeComplexParBoolTest" };
		BoolParam params[6] = { BoolParam(Tester::GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(Tester::GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			BoolParam(Tester::GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(Tester::GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE) };
		std::function<int(std::bitset<INPUTS> in)> func[2] = { [](std::bitset<INPUTS> in) { return (in[0] | ~in[1]) & ((in[0] ^ in[4]) | (in[3] & ~in[2])); }, [](std::bitset<INPUTS> in) { return (((in[0] & ~in[1]) | (in[2] ^ in[3])) & ((in[4] | in[5]) & (~in[6] | (in[0] & in[1])))) | (((in[2] & in[3]) | (in[4] ^ in[5])) & ((in[6] | ~in[0]) & (in[1] | in[2]))); } };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, std::function<int(std::bitset<INPUTS> in)> boolFunc, int THREAD_NUM) {
			Timer testTimer("boolTestTimer");

			omp_set_num_threads(THREAD_NUM);
			
			BoolProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE, boolFunc);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		ParBoolTester() : Tester("ParBoolTest") {
			for (int f = 0; f < (sizeof(boolFuncs) / sizeof(*boolFuncs)); f++) {
				for (int t = 0; t < (sizeof(threadNums) / sizeof(*threadNums)); t++) {
					for (int i = 0; i < ROUNDS; i++) {
						if (f < 3)
							test(boolFuncs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, func[0], threadNums[t]);
						else
							test(boolFuncs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, func[1], threadNums[t]);
					}
				}
			}
		}
	};

	class SeqParityTester : private Tester
	{
	private:
		std::string parityFuncs[3] = { "smallSeqParityTest", "mediumSeqParityTest", "largeSeqParityTest" };
		BoolParam params[3] = { BoolParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE) {
			Timer testTimer("parityTestTimer");

			ParityProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		SeqParityTester() : Tester("SeqParityTest") {
			for (int f = 0; f < (sizeof(parityFuncs) / sizeof(*parityFuncs)); f++)
				for (int i = 0; i < ROUNDS; i++)
					test(parityFuncs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop);
		}
	};

	class ParParityTester : private Tester
	{
	private:
		std::string parityFuncs[3] = { "smallParParityTest", "mediumParParityTest", "largeParParityTest" };
		BoolParam params[3] = { BoolParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int THREAD_NUM) {
			Timer testTimer("parityTestTimer");

			omp_set_num_threads(THREAD_NUM);

			ParityProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		ParParityTester() : Tester("ParParityTest") {
			for (int f = 0; f < (sizeof(parityFuncs) / sizeof(*parityFuncs)); f++)
				for (int t = 0; t < (sizeof(threadNums) / sizeof(*threadNums)); t++)
					for (int i = 0; i < ROUNDS; i++)
						test(parityFuncs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, threadNums[t]);
		}
	};
}

#endif