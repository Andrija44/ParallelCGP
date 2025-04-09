#ifndef PROBLEM
#define PROBLEM
#define TYPE double

#include "CGPIndividual.h"
#include <cmath>
#include <random>

namespace parallel_cgp {
	class Problem {
	private:
        /**
         * Broj jedinki u generaciji.
         */
        int POPULATION_SIZE = 20;
        /**
         * Naziv datoteke koja sadrzi najbolju jedinku.
         */
        std::string bestFile = "problem_best.txt";
        int NUM_OPERANDS = 9;
        int BI_OPERANDS = 5;
        int GENERATIONS = 5000;
        int ROWS = 20;
        int COLUMNS = 20;
        int LEVELS_BACK = 3;
        int INPUTS = 6;
        int OUTPUTS = 1;
        int MUTATIONS = 6;

        virtual TYPE computeNode(int operand, TYPE value1, TYPE value2);
        virtual TYPE fitness(TYPE fit);
		virtual void problemController(parallel_cgp::CGPIndividual &individual, TYPE &fit);
        virtual std::string evalFunction(int CGPNodeNum);
	public:
		virtual void problemSimulator();
        virtual void printFunction();
	};
}

#endif