#ifndef TOWERPROBLEM_H
#define TOWERPROBLEM_H

#include "../Problem.h"
#include "../cgp/CGP.h"
#include <bitset>
#include <vector>

#undef TYPE
#define TYPE long

namespace parallel_cgp {
	class TowerProblem : public Problem {
	private:
		/**
		 * Najbolja jedinka nakon pokretanja problem simulatora.
		 */
		CGPIndividual bestI;
		/**
		 * Naziv datoteke koja sadrzi najbolju jedinku.
		 */
		const std::string bestFile = "tower_best.txt";

		/**
		 * Nepromjenjivi parametri za ovaj problem.<br>
		 * Operandi jer ovise o funkcijama.<br>
		 * A broj inputa i outputa jer o njemu ovisi funkcija koja se trazi.<br>
		 * Disk num oznacava broj diskova u igri.<br>
		 * Tower num oznacava broj tornjeva.
		 */
		const static int NUM_OPERANDS = 9;
		const static int BI_OPERANDS = 5;
		const static int INPUTS = 3;
		const static int OUTPUTS = 2;
		const static int DISK_NUM = 5;
		const static int TOWER_NUM = 3;

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
		 * Funkcija koja oznacava optimalan broj koraka za rjesavanje tower problema.
		 */
		const std::function<TYPE()> towerFunc =
			[&]() { return (static_cast<TYPE>(pow(2, DISK_NUM)) - 1); };

		TYPE computeNode(int operand, TYPE value1, TYPE value2);
		TYPE fitness(TYPE res, bool illegalMove, bool reachedEnd);
		void problemSimulator(parallel_cgp::CGPIndividual& individual, TYPE& fit);
		std::string evalFunction(int CGPNodeNum) override;
		bool testMove(std::vector<std::bitset<DISK_NUM>> &bits, int from, int to);
	public:
		/**
		 * Osnovni kostruktor koji kreira osnovnu jedinku na bazi prije zadanih vrijednosti.
		 */
		TowerProblem() {};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za tower problem.
		 */
		TowerProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int MUTATIONS, int POPULATION_SIZE)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), MUTATIONS(MUTATIONS),
			POPULATION_SIZE(POPULATION_SIZE) {};

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
