#ifndef FUNCPROBLEM_H
#define FUNCPROBLEM_H

#include "../Problem.h"
#include "../cgp/CGP.h"

#undef TYPE
#define TYPE double

namespace parallel_cgp {
	/**
	 * Klasa koja opisuje problem pronalaska funkcije.
	 */
	class FuncProblem : public Problem {
	private:
		/**
		 * Naziv datoteke koja sadrzi najbolju jedinku.
		 */
		const std::string bestFile = "func_best.txt";

		/**
		 * Nepromjenjivi parametri za ovaj problem.<br>
		 * Operandi jer ovise o funkcijama.<br>
		 * A broj inputa i outputa jer o njemu ovisi funkcija koja se trazi.
		 */
		const static int NUM_OPERANDS = 9;
		const static int BI_OPERANDS = 5;
		const static int INPUTS = 2;
		const static int OUTPUTS = 1;
		const static int THRESHOLD = 40;

		/**
		 * Promjenjivi parametri za ovaj problem.<br>
		 * Svi su detaljno opisani u CGP klasi.
		 */
		int GENERATIONS = 5000;
		int ROWS = 8;
		int COLUMNS = 8;
		int LEVELS_BACK = 1;
		int MUTATIONS = 0;
		int POPULATION_SIZE = 20;

		/**
		 * Parametar koji oznacava je li simulacija obavljena.
		 */
		bool isSimulated = false;

		/**
		 * Funkcija koja oznacava funkciju koju CGP pokusava pronaci.
		 */
		const std::function<TYPE(TYPE x, TYPE y)> func =
			[](TYPE x, TYPE y) { return (pow(x, 2) + 2 * x * y + y); };

		TYPE computeNode(int operand, TYPE value1, TYPE value2) override;
		TYPE fitness(TYPE x, TYPE y, TYPE res);
		void problemSimulator(parallel_cgp::CGPIndividual& individual, TYPE& fit) override;
		std::string evalFunction(int CGPNodeNum) override;
	public:
		/**
		 * Osnovni kostruktor koji kreira osnovnu jedinku na bazi prije zadanih vrijednosti.
		 */
		FuncProblem() {};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za func problem.
		 */
		FuncProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int MUTATIONS, int POPULATION_SIZE)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), MUTATIONS(MUTATIONS), POPULATION_SIZE(POPULATION_SIZE) {};

		/**
		 * Metoda za pokretanje problema.
		 */
		void problemRunner() override;
		/**
		 * Metoda za ispis na kraju dobivene funkcije.
		 */
		void printFunction() override;
	};
}

#endif