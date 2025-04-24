#ifndef ADPROBLEM_HPP
#define ADPROBLEM_HPP

#include "../Problem.hpp"
#include "../cgp/CGP.hpp"

#undef TYPE
#define TYPE int

namespace parallel_cgp {
	/**
	 * Klasa koja predstavlja problem igranja Acey Deucey igre.
	 */
	class ADProblem : public Problem {
	private:
		/**
		 * Nepromjenjivi parametri za Acey Deucey problem.<br>
		 * Broj operanada koji se koriste.<br>
		 * Broj inputa i outputa jer o njima ovisi funkcija koja se trazi.<br>
		 * Pocetni novci i maksimalni novci za igru.<br>
		 * Broj setova karata koji se koristi ucenje.<br>
		 * Tocka u kojoj se igra prekida.
		 */
		const static int NUM_OPERANDS = 4;
		const static int BI_OPERANDS = 4;
		const static int INPUTS = 2;
		const static int OUTPUTS = 1;
		const static int MAX_CASH = 1000;
		const static int STARTING_CASH = 100;
		const static int CARD_SETS = 500;
		const static int THRESHOLD = STARTING_CASH * 3;

		/**
		 * Promjenjivi parametri za ovaj problem.<br>
		 * Svi su detaljno opisani u CGP klasi.
		 */
		int GENERATIONS = 200;
		int ROWS = 8;
		int COLUMNS = 8;
		int LEVELS_BACK = 3;
		int POPULATION_SIZE = 15;

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
		 * Osnovni kostruktor koji kreira osnovnu jedinku na bazi prije zadanih vrijednosti.
		 */
		ADProblem() {};
		/**
		 * Konstruktor koji prima sve promjenjive vrijednosti za Acey Deucey problem.
		 */
		ADProblem(int GENERATIONS, int ROWS, int COLUMNS, int LEVELS_BACK, int POPULATION_SIZE)
			: GENERATIONS(GENERATIONS), ROWS(ROWS), COLUMNS(COLUMNS), LEVELS_BACK(LEVELS_BACK), POPULATION_SIZE(POPULATION_SIZE) {};

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
