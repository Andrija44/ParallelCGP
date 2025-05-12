#ifndef TESTER_HPP
#define TESTER_HPP

#include "Timer.hpp"
#include <omp.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#ifndef _OPENMP
#define omp_set_num_threads(threads) 0
#endif

namespace parallel_cgp {
	/** 
	 * Klasa koja opisuje jedan Tester problema.
	 */
	class Tester
	{
	private:
		std::string testerName;
		std::string filename;
	public:
		/**
		 * Naziv verzije programa.
		 * @note Ova varijabla se koristi za naziv datoteke koja se stvara za svaki tester.
		 */
		inline static std::string VERSION_NAME = "";

		/** @name Vrijednosti testera
		 * Vrijednosti koje se koriste kod razlicitih testova.
		 */
		/**@{*/
		/** Koliko se puta vrti jedan test. */
		const static int ROUNDS = 10;
		/** Broj generacija po testu. */
		const static int GENERATIONS = 1000;
		/** Broj CGP redova za mali test. */
		const static int SMALL_ROWS = 4;
		/** Broj CGP redova za srednji test. */
		const static int MEDIUM_ROWS = 8;
		/** Broj CGP redova za veliki test. */
		const static int LARGE_ROWS = 10;
		/** Broj CGP stupaca za poseban test. */
		const static int SPECIAL_ROWS = 1;
		/** Broj CGP stupaca za mali test. */
		const static int SMALL_COLUMNS = 4;
		/** Broj CGP stupaca za srednji test. */
		const static int MEDIUM_COLUMNS = 8;
		/** Broj CGP stupaca za veliki test. */
		const static int LARGE_COLUMNS = 10;
		/** Broj CGP stupaca za poseban test. */
		const static int SPECIAL_COLUMNS = 100;
		/** Broj CGP razina unatrag za mali test (CGPIndividual#levelsBack). */
		const static int SMALL_LEVELS = 0;
		/** Broj CGP razina unatrag za srednji test (CGPIndividual#levelsBack). */
		const static int MEDIUM_LEVELS = 1;
		/** Broj CGP razina unatrag za veliki test (CGPIndividual#levelsBack). */
		const static int LARGE_LEVELS = 3;
		/** Broj CGP razina unatrag za poseban test (CGPIndividual#levelsBack). */
		const static int SPECIAL_LEVELS = 10;
		/** Velicina populacije za mali test. */
		const static int SMALL_POP_SIZE = 5;
		/** Velicina populacije za srednji test. */
		const static int MEDIUM_POP_SIZE = 8;
		/** Velicina populacije za veliki test. */
		const static int LARGE_POP_SIZE = 16;
		/** Velicina populacije za poseban test. */
		const static int SPECIAL_POP_SIZE = 5;
		/** Koje ce se sve kolicine dretvi koristiti u testovima. */
		inline static std::vector<int> threadNums = { 1, 2, 4, 8, 16 };
		/**@}*/

		/**
		 * Konstruktor koji incijalizira varijable i stvara csv datoteku za tu instancu.
		 * @param[in] testerName Naziv test suitea.
		 */
		Tester(std::string testerName) : testerName(testerName), filename(testerName) {
			filename.append(VERSION_NAME);
			filename.append(".csv");
			std::ofstream myFile;
			myFile.open(filename);
			myFile.close();
		}

		/**
		 * Funkcija koja sprema sve rezultate u datoteku te ispisuje trenutno stanje testiranja.
		 * @param[in] testName Naziv trenutnog testa.
		 */
		void saveResults(std::string testName, int gens, int rows, int cols, int levels, int pop) {
			Timer::saveTimes(filename, testName, gens, rows, cols, levels, pop);

			std::cout << "--------------------------------------" << std::endl;
			std::cout << "TEST NAME: " << testName << std::endl;
			std::cout << "--------------------------------------" << std::endl;
			std::cout << "GENS: " << gens << ", ROWS: " << rows << ", COLUMNS: " << cols
				<< ", LEVELS BACK: " << levels << ", POP SIZE: " << pop << std::endl;
			std::cout << "--------------------------------------" << std::endl;
			Timer::clearTimes();
		}
	};
}

#endif
