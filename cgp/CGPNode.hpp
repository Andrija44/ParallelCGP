#ifndef CGPNODE_HPP
#define CGPNODE_HPP
#include <iostream>
#include <fstream>
#include <string>
#define TYPE double

namespace parallel_cgp {
    /**
     * Struktura koja opisuje gene mreze CGP jedinke.
     */
    struct CGPNode {
        /**
         * Vrijednost koja oznacava koji se operand koristi u nodeu.
         */
        int operand;
        /**
         * Prva konekcija nodea na drugi node.
         */
        int connection1;
        /**
         * Druga konekcija nodea na drugi node.
         */
        int connection2;
        /**
         * Vrijednost koja oznacava koristi li se node.
         */
        bool used;
        /**
         * Izlazna vrijednost nakon racunanja vrijednosti.
         */
        TYPE outValue;
    };
}

#endif