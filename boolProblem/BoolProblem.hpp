#ifndef BOOLPROBLEM_HPP
#define BOOLPROBLEM_HPP

#include "../Problem.hpp"
#include "../cgp/CGP.hpp"
#include <bitset>

#undef TYPE
#define TYPE int

namespace parallel_cgp {
	/**
	 * Klasa koja opisuje problem pronalaska boolean funkcije.
	 */
	class BoolProblem : public Problem {
	protected:
		/**
		 * Nepromjenjivi parametri za ovaj problem.<br>
		 * Operandi jer ovise o funkcijama.<br>
		 * A broj inputa i outputa jer o njemu ovisi funkcija koja se trazi.
		 */
		const static int NUM_OPERANDS = 4;
		const static int BI_OPERANDS = 4;
		const static int INPUTS = 7;
		const static int OUTPUTS = 1;

		/**
		 * Promjenjivi parametri za ovaj problem.<br>
		 * Svi su detaljno opisani u CGP klasi.
		 */
		int GENERATIONS = 5000;
		int ROWS = 10;
		int COLUMNS = 10;
		int LEVELS_BACK = 3;
		int POPULATION_SIZE = 15;

		/**
		 * Parametar koji oznacava je li simulacija obavljena.
		 */
		bool isSimulated = false;
		/**
		 * Parametar koji oznacava koristi li se funkcija ili partiet.
		 */
		bool useFunc = true;

		/**
		 * Boolean funkcija koja oznacava funkciju koju CGP pokusava pronaci.
		 */
		std::function<int(std::bitset<INPUTS> in)> boolFunc = 
			[](std::bitset<INPUTS> in) { return (in[0] | ~in[1]) & ((in[0] ^ in[4]) | (in[3] & ~in[2])); };
		/**
		 * Parity 8bit funkcija koju CGP pokusava pronaci.
		 */
		std::function<int(std::bitset<INPUTS> in)> parityFunc =
			[](std::bitset<INPUTS> in) { return (in.count() % 2 == 0) ? 0 : 1; };

		TYPE computeNode(int operand, TYPE value1, TYPE value2);
		TYPE fitness(std::bitset<INPUTS> input, TYPE res);
		void problemSimulator(CGPIndividual &individual, TYPE &fit);
		std::string evalFunction(int CGPNodeNum) override;
	public:
		/**
		 * Osnovni kostruktor koji kreira osnovnu jedinku na bazi prije zadanih vrijednosti.
		 */
		BoolProblem() {};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za bool problem osim funkcije.<br>
		 * Primarno se koristi kod kreacije ParityProblem klase.
		 */
		BoolProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), POPULATION_SIZE(POPULATION_SIZE) {
		};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za bool problem.
		 */
		BoolProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, std::function<int(std::bitset<INPUTS> in)> boolFunc)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), POPULATION_SIZE(POPULATION_SIZE), boolFunc(boolFunc) {};

		/**
		 * Metoda za pokretanje problema.
		 */
		void problemRunner() override;
		/**
		 * Metoda za ispis na kraju dobivene funkcije.
		 */
		void printFunction() override;
	};

	/**
	 * Klasa koja opisuje problema pariteta.
	 */
	class ParityProblem : public BoolProblem {
	public:
		/**
		 * Konstruktor koji samo mijenja koja se funkcija koristi.
		 */
		ParityProblem() : BoolProblem() { useFunc = false; };
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za parity problem.
		 */
		ParityProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE)
			: BoolProblem(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, POPULATION_SIZE) { useFunc = false; };
	};
}

#endif