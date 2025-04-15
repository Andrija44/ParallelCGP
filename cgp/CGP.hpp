#ifndef CGP_HPP
#define CGP_HPP
#define TYPE double

#include "CGPIndividual.hpp"
#include "../Timer.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <omp.h>

namespace parallel_cgp {
    /**
     * Klasa koja opisuje CGP instancu.
     */
    class CGP {
    private:
        int rows, columns, levelsBack, inputs, outputs, operands, biOperands, populationSize;
    public:
        /**
         * Konstruktor za CGP klasu.
         * @param[in] rows              Broj redova CGP mreze.
         * @param[in] columns           Broj stupaca CGP mreze.
         * @param[in] levelsBack        Broj stupaca ispred noda na koje se moze spojiti.
         * @param[in] inputs            Broj ulaznih nodova.
         * @param[in] outputs           Broj izlaznih nodova.
         * @param[in] operands          Broj operanada koji su na raspolaganju.
         * @param[in] biOperands        Broj prvog operanda koji prima jedan ulaz.
         * @param[in] populationSize    Broj jedinki u populaciji.
         */
        CGP(int rows, int columns, int levelsBack, int inputs, int outputs, int operands, int biOperands, int populationSize)
            : rows(rows), columns(columns), levelsBack(levelsBack), inputs(inputs), outputs(outputs), 
                operands(operands), biOperands(biOperands), populationSize(populationSize) {};

        /**
         * Funkcija za generiranje inicijalne populacije.<br>
         * Broj jedinki u populaciji ovisi o konstanti POPULATION_SIZE.<br>
         * Ostali parametri su navedeni u konstruktoru.
         */
        void generatePopulation(std::vector<CGPIndividual> &population);

        /**
         * Funkcija za kreiranje nove generacije populacije na bazi roditeljske jedinke.<br>
         * Koristi se <b>Goldman Mutacija</b> kojom se u roditeljskoj jedinci mutiraju geni
         * sve dok se ne dode do gena koji se aktivno koristi.
         * Taj gen se jos promjeni i s njime zavrsava mutacija nove jedinke.
         * @param[in] parent    Najbolja jedinka iz prosle generacija, roditelj za novu.
         */
        void goldMutate(CGPIndividual parent, std::vector<CGPIndividual> &population);
    };
}

#endif
