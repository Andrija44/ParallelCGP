#ifndef BOOLTESTER_HPP
#define BOOLTESTER_HPP

#include "../Tester.hpp"
#include "../Timer.hpp"
#include "BoolProblem.hpp"

namespace parallel_cgp {
	/**
	 * Struktura koja se koristi za upravljanje test parametara.
	 */
	struct BoolParam {
		BoolParam() {}
		BoolParam(int gens, int rows, int cols, int levels, int pop) : gens(gens), rows(rows), cols(cols), levels(levels), pop(pop) {}
		int gens;
		/** Broj redova za CGP. */
		int rows;
		/** Broj stupaca za CGP. */
		int cols;
		/** Broj razina iza na koliko se nodeovi mogu spajati u CGP. */
		int levels;
		/** Velicina populacije. */
		int pop;
	};

	/**
	 * Klasa koja opisuje sekvencijski tester Bool problema.
	 */
	class SeqBoolTester : private Tester, private BoolProblem
	{
	private:
		std::string boolFuncs[8] = { "smallSimpleSeqBoolTest", "mediumSimpleSeqBoolTest", "largeSimpleSeqBoolTest", "specialSimpleSeqBoolTest", "smallComplexSeqBoolTest", "mediumComplexSeqBoolTest", "largeComplexSeqBoolTest", "specialComplexSeqBoolTest" };
		BoolParam params[8] = { BoolParam(Tester::GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(Tester::GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			BoolParam(Tester::GENERATIONS, SPECIAL_ROWS, SPECIAL_COLUMNS, SPECIAL_LEVELS, SPECIAL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(Tester::GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			BoolParam(Tester::GENERATIONS, SPECIAL_ROWS, SPECIAL_COLUMNS, SPECIAL_LEVELS, SPECIAL_POP_SIZE) };
		std::function<int(std::bitset<INPUTS> in)> func[2] = { [](std::bitset<INPUTS> in) { return (in[0] | ~in[1]) & ((in[0] ^ in[4]) | (in[3] & ~in[2])); }, [](std::bitset<INPUTS> in) { return (((in[0] & ~in[1]) | (in[2] ^ in[3])) & ((in[4] | in[5]) & (~in[6] | (in[0] & in[1])))) | (((in[2] & in[3]) | (in[4] ^ in[5])) & ((in[6] | ~in[0]) & (in[1] | in[2]))); } };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, std::function<int(std::bitset<INPUTS> in)> boolFunc) {
			Timer testTimer("boolTestTimer");

			BoolProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE, boolFunc);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		/**
		 * Konstruktor testera koji odmah i pokrece testiranje.<br>
		 * Parametar ROUNDS je opisan u Tester.
		 */
		SeqBoolTester(BoolParam customParams) : Tester((customParams.pop == 0) ? "SeqBoolTest" : "CustomSeqBoolTest") {
			if(customParams.pop != 0) {
				for(int i = 0; i < ROUNDS; i++)
					test("CustomSeqBoolTest", customParams.gens, customParams.rows, customParams.cols, customParams.levels, customParams.pop, func[0]);
				return;
			}
			
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

	/**
	 * Klasa koja opisuje paralelni tester Bool problema.
	 */
	class ParBoolTester : private Tester, private BoolProblem
	{
	private:
		std::string boolFuncs[8] = { "smallSimpleParBoolTest", "mediumSimpleParBoolTest", "largeSimpleParBoolTest", "specialSimpleParBoolTest", "smallComplexParBoolTest", "mediumComplexParBoolTest", "largeComplexParBoolTest", "specialComplexParBoolTest" };
		BoolParam params[8] = { BoolParam(Tester::GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(Tester::GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			BoolParam(Tester::GENERATIONS, SPECIAL_ROWS, SPECIAL_COLUMNS, SPECIAL_LEVELS, SPECIAL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(Tester::GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(Tester::GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			BoolParam(Tester::GENERATIONS, SPECIAL_ROWS, SPECIAL_COLUMNS, SPECIAL_LEVELS, SPECIAL_POP_SIZE) };
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
		/**
		 * Konstruktor testera koji odmah i pokrece testiranje.<br>
		 * Parametar ROUNDS je opisan u Tester.
		 */
		ParBoolTester(BoolParam customParams) : Tester((customParams.pop == 0) ? "ParBoolTest" : "CustomParBoolTest") {
			if(customParams.pop != 0) {
				for (int t = 0; t < threadNums.size(); t++) {
					for(int i = 0; i < ROUNDS; i++)
						test("CustomParBoolTest", customParams.gens, customParams.rows, customParams.cols, customParams.levels, customParams.pop, func[0], threadNums[t]);
					return;
				}
			}

			for (int f = 0; f < (sizeof(boolFuncs) / sizeof(*boolFuncs)); f++) {
				for (int t = 0; t < threadNums.size(); t++) {
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

	/**
	 * Klasa koja opisuje sekvencijski tester Parity problema.
	 */
	class SeqParityTester : private Tester
	{
	private:
		std::string parityFuncs[4] = { "smallSeqParityTest", "mediumSeqParityTest", "largeSeqParityTest", "specialSeqParityTest" };
		BoolParam params[4] = { BoolParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			BoolParam(GENERATIONS, SPECIAL_ROWS, SPECIAL_COLUMNS, SPECIAL_LEVELS, SPECIAL_POP_SIZE) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE) {
			Timer testTimer("parityTestTimer");

			ParityProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		/**
		 * Konstruktor testera koji odmah i pokrece testiranje.<br>
		 * Parametar ROUNDS je opisan u Tester.
		 */
		SeqParityTester(BoolParam customParams) : Tester((customParams.pop == 0) ? "SeqParityTest" : "CustomSeqParityTest") {
			if(customParams.pop != 0) {
				for(int i = 0; i < ROUNDS; i++)
					test("CustomSeqParityTest", customParams.gens, customParams.rows, customParams.cols, customParams.levels, customParams.pop);
				return;
			}

			for (int f = 0; f < (sizeof(parityFuncs) / sizeof(*parityFuncs)); f++)
				for (int i = 0; i < ROUNDS; i++)
					test(parityFuncs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop);
		}
	};

	/**
	 * Klasa koja opisuje paralelni tester Parity problema.
	 */
	class ParParityTester : private Tester
	{
	private:
		std::string parityFuncs[4] = { "smallParParityTest", "mediumParParityTest", "largeParParityTest", "specialParParityTest" };
		BoolParam params[4] = { BoolParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			BoolParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			BoolParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			BoolParam(GENERATIONS, SPECIAL_ROWS, SPECIAL_COLUMNS, SPECIAL_LEVELS, SPECIAL_POP_SIZE) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int THREAD_NUM) {
			Timer testTimer("parityTestTimer");

			omp_set_num_threads(THREAD_NUM);

			ParityProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		/**
		 * Konstruktor testera koji odmah i pokrece testiranje.<br>
		 * Parametar ROUNDS je opisan u Tester.
		 */
		ParParityTester(BoolParam customParams) : Tester((customParams.pop == 0) ? "ParParityTest" : "CustomParParityTest") {
			if(customParams.pop != 0) {
				for (int t = 0; t < threadNums.size(); t++) {
					for(int i = 0; i < ROUNDS; i++)
						test("CustomParParityTest", customParams.gens, customParams.rows, customParams.cols, customParams.levels, customParams.pop, threadNums[t]);
					return;
				}
			}

			for (int f = 0; f < (sizeof(parityFuncs) / sizeof(*parityFuncs)); f++)
				for (int t = 0; t < threadNums.size(); t++)
					for (int i = 0; i < ROUNDS; i++)
						test(parityFuncs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, threadNums[t]);
		}
	};
}

#endif