#ifndef BOOLPROBLEM
#define BOOLPROBLEM

#include "Problem.h"
#include "CGP.h"
#include <bitset>

#undef TYPE
#define TYPE int

namespace parallel_cgp {
	/**
	 * Klasa koja opisuje problem pronalaska boolean funkcije.
	 */
	class BoolProblem : public Problem {
	private:
		/**
		 * Najbolja jedinka nakon pokretanja problem simulatora.
		 */
		CGPIndividual bestI;
		/**
		 * Naziv datoteke koja sadrzi najbolju jedinku.
		 */
		const std::string bestFile = "bool_best.txt";

		/**
		 * Nepromjenjivi parametri za ovaj problem.<br>
		 * Operandi jer ovise o funkcijama.<br>
		 * A broj inputa jer o njemu ovisi funkcija koja se trazi.
		 */
		const static int NUM_OPERANDS = 4;
		const static int BI_OPERANDS = 4;
		const static int INPUTS = 5;

		/**
		 * Promjenjivi parametri za ovaj problem.<br>
		 * Svi su detaljno opisani u CGP klasi.
		 */
		int GENERATIONS = 5000;
		int ROWS = 8;
		int COLUMNS = 8;
		int LEVELS_BACK = 1;
		int OUTPUTS = 1;
		int MUTATIONS = 0;
		int POPULATION_SIZE = 20;

		/**
		 * Parametar koji oznacava je li simulacija obavljena.
		 */
		bool isSimulated = false;

		/**
		 * Boolean funkcija koja oznacava funkciju koju CGP pokusava pronaci.
		 */
		const std::function<int(std::bitset<INPUTS> in)> boolFunc = 
			[](std::bitset<INPUTS> in) { return (in[0] | ~in[1]) & (in[0] ^ in[4] | (in[3] & ~in[2])); };

		TYPE computeNode(int operand, TYPE value1, TYPE value2);
		TYPE fitness(std::bitset<INPUTS> input, int res);
		void problemController(CGPIndividual &individual, TYPE &fit);
		std::string evalFunction(int CGPNodeNum) override;
	public:
		/**
		 * Osnovni kostruktor koji kreira osnovnu jedinku na bazi prije zadanih vrijednosti.
		 */
		BoolProblem() {};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za bool problem.
		 */
		BoolProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int OUTPUTS, int MUTATIONS, int POPULATION_SIZE)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), OUTPUTS(OUTPUTS), MUTATIONS(MUTATIONS), POPULATION_SIZE(POPULATION_SIZE) {};

		/**
		 * Metoda za pokretanje simulacije, tj. za pokretanje problema.
		 */
		void problemSimulator() override;
		/**
		 * Metoda za ispis na kraju dobivene funkcije.
		 */
		void printFunction() override;
	};
}

#endif