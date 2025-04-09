#ifndef FUNCPROBLEM_H
#define FUNCPROBLEM_H
#define TYPE double

#include "Problem.h"
#include "CGP.h"

namespace parallel_cgp {

	class FuncProblem : public Problem {
	private:
		/**
		 * Najbolja jedinka nakon pokretanja problem simulatora.
		 */
		CGPIndividual bestI;
		/**
		 * Naziv datoteke koja sadrzi najbolju jedinku.
		 */
		const std::string bestFile = "func_best.txt";

		/**
		 * Nepromjenjivi parametri za ovaj problem.<br>
		 * Operandi jer ovise o funkcijama.<br>
		 * A broj inputa jer o njemu ovisi funkcija koja se trazi.
		 */
		const static int NUM_OPERANDS = 6;
		const static int BI_OPERANDS = 5;
		const static int INPUTS = 2;

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
		 * Funkcija koja oznacava funkciju koju CGP pokusava pronaci.
		 */
		const std::function<TYPE(TYPE x, TYPE y)> func =
			[](TYPE x, TYPE y) { return (pow(x, 2) + 2 * x * y + y); };

		TYPE computeNode(int operand, TYPE value1, TYPE value2);
		TYPE fitness(TYPE x, TYPE y, TYPE res);
		void problemController(parallel_cgp::CGPIndividual& individual, TYPE& fit) override;
		std::string evalFunction(int CGPNodeNum) override;
	public:
		/**
		 * Osnovni kostruktor koji kreira osnovnu jedinku na bazi prije zadanih vrijednosti.
		 */
		FuncProblem() {};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za func problem.
		 */
		FuncProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int OUTPUTS, int MUTATIONS, int POPULATION_SIZE)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), OUTPUTS(OUTPUTS), MUTATIONS(MUTATIONS), POPULATION_SIZE(POPULATION_SIZE) {
		};

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