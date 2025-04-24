#ifndef WAITPROBLEM_HPP
#define WAITPROBLEM_HPP

#include "../Problem.hpp"
#include "../cgp/CGP.hpp"
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
		 * Promjenjivi parametri za ovaj problem.<br>
		 * Svi su detaljno opisani u CGP klasi.
		 */
		int GENERATIONS = 200;
		int ROWS = 8;
		int COLUMNS = 8;
		int LEVELS_BACK = 3;
		int POPULATION_SIZE = 15;
		int INPUTS = 1;
		int OUTPUTS = 1;

		/**
		 * Varijabla koja opisuje koliko ce se cekati u milisekundama.
		 */
		int WAIT_TIME = 1;

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
		void problemSimulator(CGPIndividual& individual, TYPE& fit) override;
		std::string evalFunction(int CGPNodeNum) override;
	public:
		/**
		 * Osnovni kostruktor koji kreira osnovnu jedinku na bazi prije zadanih vrijednosti.
		 */
		WaitProblem() {};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za wait problem.
		 */
		WaitProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE, int WAIT_TIME)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), POPULATION_SIZE(POPULATION_SIZE), WAIT_TIME(WAIT_TIME) {};

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