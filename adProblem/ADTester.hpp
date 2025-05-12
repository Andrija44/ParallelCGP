#ifndef ADTESTER_HPP
#define ADTESTER_HPP

#include "../Tester.hpp"
#include "../Timer.hpp"
#include "ADProblem.hpp"

namespace parallel_cgp {
	/**
	 * Struktura koja se koristi za upravljanje test parametara.
	 */
	struct ADParam {
		ADParam() {}
		ADParam(int gens, int rows, int cols, int levels, int pop) : gens(gens), rows(rows), cols(cols), levels(levels), pop(pop) {}
		/** Broj generacija po testu. */
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
	 * Klasa koja opisuje sekvencijski tester Acey Deucey problema.
	 */
	class SeqADTester : private Tester
	{
	private:
		std::string funcs[4] = { "smallSeqADTest", "mediumSeqADTest", "largeSeqADTest", "specialSeqADTest" };
		ADParam params[4] = { ADParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			ADParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			ADParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			ADParam(GENERATIONS, SPECIAL_ROWS, SPECIAL_COLUMNS, SPECIAL_LEVELS, SPECIAL_POP_SIZE) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE) {
			Timer testTimer("adTestTimer");

			ADProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		/**
		 * Konstruktor testera koji odmah i pokrece testiranje.<br>
		 * Parametar ROUNDS je opisan u Tester.
		 */
		SeqADTester(ADParam customParams) : Tester((customParams.pop == 0) ? "SeqADTest" : "CustomSeqADTest") {
			if(customParams.pop != 0) {
				for(int i = 0; i < ROUNDS; i++)
					test("CustomSeqADTest", customParams.gens, customParams.rows, customParams.cols, customParams.levels, customParams.pop);
				return;
			}

			for (int f = 0; f < (sizeof(funcs) / sizeof(*funcs)); f++) {
				for (int i = 0; i < ROUNDS; i++) {
					test(funcs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop);
				}
			}
		}
	};

	/**
	 * Klasa koja opisuje paralelni tester Acey Deucey problema.
	 */
	class ParADTester : private Tester
	{
	private:
		std::string funcs[4] = { "smallParADTest", "mediumParADTest", "largeParADTest", "specialParADTest" };
		ADParam params[4] = { ADParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE),
			ADParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE),
			ADParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE),
			ADParam(GENERATIONS, SPECIAL_ROWS, SPECIAL_COLUMNS, SPECIAL_LEVELS, SPECIAL_POP_SIZE) };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int THREAD_NUM) {
			Timer testTimer("adTestTimer");

			omp_set_num_threads(THREAD_NUM);

			ADProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		/**
		 * Konstruktor testera koji odmah i pokrece testiranje.<br>
		 * Parametar ROUNDS je opisan u Tester.hpp.
		 */
		ParADTester(ADParam customParams) : Tester((customParams.pop == 0) ? "ParADTest" : "CustomParADTest") {
			if(customParams.pop != 0) {
				for (int t = 0; t < threadNums.size(); t++) {
					for(int i = 0; i < ROUNDS; i++)
						test("CustomParADTest", customParams.gens, customParams.rows, customParams.cols, customParams.levels, customParams.pop, threadNums[t]);
					return;
				}
			}

			for (int f = 0; f < (sizeof(funcs) / sizeof(*funcs)); f++) {
				for (int t = 0; t < threadNums.size(); t++) {
					for (int i = 0; i < ROUNDS; i++) {
						test(funcs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, threadNums[t]);
					}
				}
			}
		}
	};
}

#endif
