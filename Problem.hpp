#ifndef PROBLEM_HPP
#define PROBLEM_HPP
#define TYPE double

#include "Timer.hpp"
#include "cgp/CGPIndividual.hpp"
#include <cmath>
#include <random>
#include <cfloat>
#include <climits>

namespace parallel_cgp {
	class Problem {
	private:
        /**
         * Metoda koja predstavlja simulator u problemu.
         * @param[in] individual    Referenca na jedinku koja se koristi.
         * @param[out] fit           Referenca na varijablu u koju se pohranjuje fitness.
         */
		virtual void problemSimulator(parallel_cgp::CGPIndividual &individual, TYPE &fit) {}
        /**
         * Rekurzivna funkcija koja se koristi kod ispisa funckije.
         * @param[in] CGPNodeNum    Broj noda na koji je spojen output.
         */
        virtual std::string evalFunction(int CGPNodeNum) = 0;
	public:
        /**
         * Destruktor Problem objekata.
         */
        virtual ~Problem() = default;
        /**
         * Najbolja jedinka nakon pokretanja problem simulatora.
         */
        CGPIndividual *bestI;

        /**
         * Varijabla koja oznacuje hoce li se ispisivati vrijednost fitnesa za svaku generaciju.
         */
        bool printGens = false;

        /** @name Promjenjivi parametri
         * Parametri koji su na raspolaganju svakom problemu.<br>
         * Mogu se mijenjati po potrebi.
         */
        /**@{*/
        /** Ukupni broj operanada. */
        int NUM_OPERANDS = 9;
        /** Broj binarnih operanada (+1 iz nekog razloga). */
        int BI_OPERANDS = 5;
        /** Broj generacija koji se vrti. */
        int GENERATIONS = 5000;
        /** Broj redova CGP mreze. */
        int ROWS = 8;
        /** Broj stupaca CGP mreze. */
        int COLUMNS = 8;
        /** Broj razina unazad na koji se nodeovi mogu spojiti u CGP mrezi. */
        int LEVELS_BACK = 3;
        /** Broj ulaza u CGP mrezu. */
        int INPUTS = 6;
        /** Broj izlaza iz CGP mrezu. */
        int OUTPUTS = 1;
        /** Velicina populacije. */
        int POPULATION_SIZE = 20;
        /**@}*/

        /**
         * Funkcija u kojoj su zapisani svi moguci operandi za dani problem.
         * @param[in] operand    Broj operanda.
         * @param[in] value1     Prva vrijednost.
         * @param[in] value2     Druga vrijednost.
         */
        virtual TYPE computeNode(int operand, TYPE value1, TYPE value2) {
            switch (operand) {
            case 1:
                return value1 + value2;
            case 2:
                return value1 - value2;
            case 3:
                return value1 * value2;
            case 4:
                return (value2 == 0) ? 0 : value1 / value2;
            case 5:
                return sin(value1);
            case 6:
                return cos(value1);
            case 7:
                return value1 > 0 ? sqrt(value1) : value1;
            case 8:
                return pow(value1, 2);
            case 9:
                return pow(2, value1);
            default:
                return 0;
            }
        }
        /**
         * Funkcija koja se koristi za izracun fitnessa za odredenu jedinku.
         */
        virtual TYPE fitness(TYPE fit) { return fit; }

        /**
         * Metoda za pokretanje problema.
         */
		virtual void problemRunner() = 0;
        /**
         * Metoda za ispis na kraju dobivene funkcije.
         */
        virtual void printFunction() = 0;
	};
}

#endif