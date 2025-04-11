#ifndef ADPROBLEM_H
#define ADPROBLEM_H

#include "../Problem.h"
#include "../cgp/CGP.h"

#undef TYPE
#define TYPE int

namespace parallel_cgp {
	/**
	 * Klasa koja predstavlja problem igranja Acey Deucey igre.
	 */
	class ADProblem : public Problem {
	private:
		/**
		 * Naziv datoteke koja sadrzi najbolju jedinku.
		 */
		const std::string bestFile = "ad_best.txt";

		/**
		 * Nepromjenjivi parametri za ovaj problem.<br>
		 * Operandi jer ovise o funkcijama.<br>
		 * A broj inputa i outputa jer o njemu ovisi funkcija koja se trazi.
		 */
		const static int NUM_OPERANDS = 4;
		const static int BI_OPERANDS = 4;
		const static int INPUTS = 2;
		const static int OUTPUTS = 1;
		const static int MAX_CASH = 1000;
		const static int STARTING_CASH = 100;
		double CARD_SETS = 500;

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
		 * Setovi karata koji se koriste u igri
		 */
		std::vector<std::vector<double>> sets;

		/**
		 * Parametar koji oznacava je li simulacija obavljena.
		 */
		bool isSimulated = false;

		TYPE computeNode(int operand, TYPE value1, TYPE value2);
		double fitness(TYPE cash, TYPE maxCash, double avgCash);
		void problemSimulator(parallel_cgp::CGPIndividual& individual, double& fit) override;
		std::string evalFunction(int CGPNodeNum) override;
	public:
		/**
		 * Metoda za pokretanje problema.
		 */
		void problemRunner() override;
		/**
		 * Metoda za ispis na kraju dobivene funkcije.
		 */
		void printFunction() override;
		/**
		 * Metoda prikaze kako najbolja jedinka igra jednu partiju igre.
		 */
		void playGame();
	};
}

#endif
