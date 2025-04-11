#ifndef CGP_H
#define CGP_H
#define TYPE double

#include <vector>
#include <string>
#include <omp.h>
#include "CGPIndividual.h"

namespace parallel_cgp {
    /**
     * Klasa koja opisuje CGP instancu.
     */
    class CGP {
    private:
        int generations, rows, columns, levelsBack, inputs, outputs, mutations, operands, biOperands, populationSize;
    public:
        /**
         * Konstruktor za CGP klasu.
         * @param[in] generations       Broj generacija koji ce se izvrtiti pri ucenju.
         * @param[in] rows              Broj redova CGP mreze.
         * @param[in] columns           Broj stupaca CGP mreze.
         * @param[in] levelsBack        Broj stupaca ispred noda na koje se moze spojiti.
         * @param[in] inputs            Broj ulaznih nodova.
         * @param[in] outputs           Broj izlaznih nodova.
         * @param[in] mutations         Broj mutacija genoma po jedinki.
         * @param[in] operands          Broj operanada koji su na raspolaganju.
         * @param[in] biOperands        Broj prvog operanda koji prima jedan ulaz.
         * @param[in] populationSize    Broj jedinki u populaciji.
         */
        CGP(int generations, int rows, int columns, int levelsBack, int inputs, int outputs, int mutations, int operands, int biOperands, int populationSize)
            : generations(generations), rows(rows), columns(columns), levelsBack(levelsBack), inputs(inputs), outputs(outputs), mutations(mutations), 
                operands(operands), biOperands(biOperands), populationSize(populationSize) {};

        /**
         * Funkcija za generiranje inicijalne populacije.<br>
         * Broj jedinki u populaciji ovisi o konstanti POPULATION_SIZE.<br>
         * Ostali parametri su navedeni u konstruktoru.
         */
        void generatePopulation(std::vector<CGPIndividual> &population);

        /**
         * Funkcija za kreiranje nove generacije populacije na bazi roditeljske jedinke.<br>
         * Koristi se <b>Point Mutacija</b> kojom se u roditeljskoj jedinci mutira dani
         * broj gena kako bi se kreirala nova jedinka.<br>
         * @param[in] parent    Najbolja jedinka iz prosle generacija, roditelj za novu.
         */
        std::vector<CGPIndividual> pointMutate(CGPIndividual parent);

        /**
         * Funkcija za kreiranje nove generacije populacije na bazi roditeljske jedinke.<br>
         * Koristi se <b>Goldman Mutacija</b> kojom se u roditeljskoj jedinci mutiraju geni
         * sve dok se ne dode do gena koji se aktivno koristi.
         * Taj gen se jos promjeni i s njime zavrsava mutacija nove jedinke.
         * @param[in] parent    Najbolja jedinka iz prosle generacija, roditelj za novu.
         */
        std::vector<CGPIndividual> goldMutate(CGPIndividual parent);
    };
}

#endif
