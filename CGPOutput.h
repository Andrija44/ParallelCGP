#ifndef CGPOUTPUT_H
#define CGPOUTPUT_H
#include <iostream>
#include <fstream>
#include <string>

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

        /**
         * Operator overloading za pisanje izlaznog gena u datoteku.
         */
        friend std::ostream& operator<<(std::ostream& os, const CGPOutput& output) {
            os << output.connection << " " << output.value;
            return os;
        }
        /**
         * Operator overloading za citanje izlaznog gena iz datoteke.
         */
        friend std::istream& operator>>(std::istream& is, CGPOutput& output) {
            is >> output.connection >> output.value;
            return is;
        }
    };
}

#endif