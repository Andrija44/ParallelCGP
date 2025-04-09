#ifndef WAITPROBLEM_H
#define WAITPROBLEM_H

#include "../Problem.h"
#include "../cgp/CGP.h"
#include <chrono>
#include <thread>

#undef TYPE
#define TYPE double

namespace parallel_cgp {
	/**
	 * Klasa koja opisuje problem koji ceka odredeno vrijeme.
	 */
	class WaitProblem : public Problem {
	private:
		/**
		 * Najbolja jedinka nakon pokretanja problem simulatora.
		 */
		CGPIndividual bestI;
		/**
		 * Naziv datoteke koja sadrzi najbolju jedinku.
		 */
		const std::string bestFile = "wait_best.txt";

		/**
		 * Promjenjivi parametri za ovaj problem.<br>
		 * Svi su detaljno opisani u CGP klasi.
		 */
		int GENERATIONS = 5000;
		int ROWS = 8;
		int COLUMNS = 8;
		int LEVELS_BACK = 1;
		int INPUTS = 1;
		int OUTPUTS = 1;
		int MUTATIONS = 0;
		int POPULATION_SIZE = 20;

		/**
		 * Varijabla koja opisuje koliko ce se cekati u milisekundama.
		 */
		int WAIT_TIME = 10;

		/**
		 * Parametar koji oznacava je li simulacija obavljena.
		 */
		bool isSimulated = false;

		/**
		 * Wiat funkcija koja oznacava cekanje.
		 */
		const std::function<void()> waitFunc =
			[&]() { std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME)); };

		TYPE fitness(TYPE prev) override;
		void problemController(CGPIndividual& individual, TYPE& fit);
		std::string evalFunction(int CGPNodeNum) override;
	public:
		/**
		 * Osnovni kostruktor koji kreira osnovnu jedinku na bazi prije zadanih vrijednosti.
		 */
		WaitProblem() {};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za wait problem.
		 */
		WaitProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int OUTPUTS, int MUTATIONS, int POPULATION_SIZE, int WAIT_TIME)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), OUTPUTS(OUTPUTS), MUTATIONS(MUTATIONS),
			POPULATION_SIZE(POPULATION_SIZE), WAIT_TIME(WAIT_TIME) {
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