#ifndef FUNCTESTER_HPP
#define FUNCTESTER_HPP

#include "../Tester.hpp"
#include "../Timer.hpp"
#include "FuncProblem.hpp"

namespace parallel_cgp {
	/**
	 * Struktura koja se koristi za upravljanje test parametara.
	 */
	struct FuncParam {
		FuncParam() {}
		FuncParam(int gens, int rows, int cols, int levels, int pop, int thresh) : gens(gens), rows(rows), cols(cols), levels(levels), pop(pop), thresh(thresh) {}
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
		/** Vrijednost nakon koje se zaustavlja problem. Ako je manja od 0 onda se gledaju generacije. */
		int thresh;
	};

	/**
	 * Klasa koja opisuje sekvencijski tester Func problema.
	 */
	class SeqFuncTester : private Tester
	{
	private:
		std::string funcs[6] = { "smallSimpleSeqFuncTest", "mediumSimpleSeqFuncTest", "largeSimpleSeqFuncTest", "smallComplexSeqFuncTest", "mediumComplexSeqFuncTest", "largeComplexSeqFuncTest" };
		FuncParam params[6] = { FuncParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE, -1),
			FuncParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE, -1),
			FuncParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE, -1),
			FuncParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE, -1),
			FuncParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE, -1),
			FuncParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE, -1) };
		std::function<TYPE(TYPE x, TYPE y)> func[2] = { [](TYPE x, TYPE y) { return (pow(x, 2) + 2 * x * y + y); } , [](TYPE x, TYPE y) { return (pow(x, 3) * sin(y) + 2 * cos(x) * pow(y, 2) + 4 * pow(x, 2) * pow(y, 3) - 3 * sin(x) * cos(y)); } };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int THRESHOLD, std::function<TYPE(TYPE x, TYPE y)> func) {
			Timer testTimer("funcTestTimer");

			FuncProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE, THRESHOLD, func);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		/**
		 * Konstruktor testera koji odmah i pokrece testiranje.<br>
		 * Parametar ROUNDS je opisan u Tester.
		 */
		SeqFuncTester(FuncParam customParams) : Tester((customParams.pop == 0) ? "SeqFuncTest" : "CustomSeqFuncTest") {
			if(customParams.pop != 0) {
				for(int i = 0; i < ROUNDS; i++)
					test("CustomSeqFuncTest", customParams.gens, customParams.rows, customParams.cols, customParams.levels, customParams.pop, customParams.thresh, func[0]);
				return;
			}

			for (int f = 0; f < (sizeof(funcs) / sizeof(*funcs)); f++) {
				for (int i = 0; i < ROUNDS; i++) {
					if (f < 3)
						test(funcs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, params[f].thresh, func[0]);
					else
						test(funcs[f], params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, params[f].thresh, func[1]);
				}
			}
		}
	};

	/**
	 * Klasa koja opisuje sekvencijski tester Func problema.
	 */
	class ParFuncTester : private Tester
	{
	private:
		std::string funcs[6] = { "smallSimpleParFuncTest", "mediumSimpleParFuncTest", "largeSimpleParFuncTest", "smallComplexParFuncTest", "mediumComplexParFuncTest", "largeComplexParFuncTest" };
		FuncParam params[6] = { FuncParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE, -1),
			FuncParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE, -1),
			FuncParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE, -1),
			FuncParam(GENERATIONS, SMALL_ROWS, SMALL_COLUMNS, SMALL_LEVELS, SMALL_POP_SIZE, -1),
			FuncParam(GENERATIONS, MEDIUM_ROWS, MEDIUM_COLUMNS, MEDIUM_LEVELS, MEDIUM_POP_SIZE, -1),
			FuncParam(GENERATIONS, LARGE_ROWS, LARGE_COLUMNS, LARGE_LEVELS, LARGE_POP_SIZE, -1) };
		std::function<TYPE(TYPE x, TYPE y)> func[2] = { [](TYPE x, TYPE y) { return (pow(x, 2) + 2 * x * y + y); } , [](TYPE x, TYPE y) { return (pow(x, 3) * sin(y) + 2 * cos(x) * pow(y, 2) + 4 * pow(x, 2) * pow(y, 3) - 3 * sin(x) * cos(y)); } };

		void test(std::string testName, int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int THRESHOLD, std::function<TYPE(TYPE x, TYPE y)> func, int THREAD_NUM) {
			Timer testTimer("funcTestTimer");

			omp_set_num_threads(THREAD_NUM);

			FuncProblem problem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE, THRESHOLD, func);
			problem.problemRunner();

			testTimer.endTimer();

			saveResults(testName, GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE);
		}
	public:
		/**
		 * Konstruktor testera koji odmah i pokrece testiranje.<br>
		 * Parametar ROUNDS je opisan u Tester.
		 */
		ParFuncTester(FuncParam customParams) : Tester((customParams.pop == 0) ? "ParFuncTest" : "CustomParFuncTest") {
			if(customParams.pop != 0) {
				for (int t = 0; t < (sizeof(threadNums) / sizeof(*threadNums)); t++) {
					for(int i = 0; i < ROUNDS; i++)
						test("CustomParFuncTest", customParams.gens, customParams.rows, customParams.cols, customParams.levels, customParams.pop, customParams.thresh, func[0], threadNums[t]);
					return;
				}
			}

			for (int f = 0; f < (sizeof(funcs) / sizeof(*funcs)); f++) {
				for (int t = 0; t < (sizeof(threadNums) / sizeof(*threadNums)); t++) {
					for (int i = 0; i < ROUNDS; i++) {
						if (f < 3)
							test(funcs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, params[f].thresh, func[0], threadNums[t]);
						else
							test(funcs[f] + std::to_string(threadNums[t]) + "T", params[f].gens, params[f].rows, params[f].cols, params[f].levels, params[f].pop, params[f].thresh, func[1], threadNums[t]);
					}
				}
			}
		}
	};
}

#endif