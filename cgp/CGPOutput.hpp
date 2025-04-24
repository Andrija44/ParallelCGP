#ifndef CGPOUTPUT_HPP
#define CGPOUTPUT_HPP
#include <iostream>
#include <fstream>
#include <string>
#define TYPE double

namespace parallel_cgp {
    /**
     * Struktura koja opisuje izlazne gene CGP jedinke.
     */
    struct CGPOutput {
        /**
         * Broj koji reprezentira na koji gen je spojen izlazni gen.
         */
        int connection;
        /**
         * Izlazna vrijednost gena nakon izracuna.
         */
        TYPE value;
    };
}

#endif