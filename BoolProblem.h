#ifndef BOOLPROBLEM
#define BOOLPROBLEM

#include "Problem.h"
#include "CGP.h"
#include <bitset>

#undef TYPE
#define TYPE int

namespace parallel_cgp {
	class BoolProblem : public Problem {
	private:
		CGPIndividual bestI;
		/**
		 * Naziv datoteke koja sadrzi najbolju jedinku.
		 */
		const std::string bestFile = "bool_best.txt";

		const static int NUM_OPERANDS = 6;
		const static int BI_OPERANDS = 6;

		/**
		 * Broj jedinki u generaciji.
		 */
		const static int POPULATION_SIZE = 20;
		const static int GENERATIONS = 5000;
		const static int ROWS = 5;
		const static int COLUMNS = 5;
		const static int LEVELS_BACK = 1;
		const static int INPUTS = 6;
		const static int OUTPUTS = 1;
		const static int MUTATIONS = 0;
		const std::function<int(std::bitset<INPUTS> in)> boolFunc = 
			[](std::bitset<INPUTS> in) { return (in[0] | ~in[1]) & (in[0] ^ in[4] | (in[3] & ~in[2])); };

		TYPE computeNode(int operand, TYPE value1, TYPE value2);
		TYPE fitness(std::bitset<INPUTS> input, int res);
		void problemController(CGPIndividual &individual, TYPE &fit);
		std::string evalFunction(int CGPNodeNum);
	public:
		void problemSimulator();
		void printFunction();
	};
}

#endif