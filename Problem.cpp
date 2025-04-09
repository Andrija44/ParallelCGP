#include "Problem.h"

using namespace std;
using namespace parallel_cgp;

TYPE Problem::computeNode(int operand, TYPE value1, TYPE value2) {
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

TYPE Problem::fitness(TYPE fit) {
    return fit;
}

void Problem::printFunction() {
    cout << "Funkcija" << endl;
}

std::string Problem::evalFunction(int CGPNodeNum) {
    std::ostringstream oss;

    if (CGPNodeNum < INPUTS)
        oss << "bit[" << CGPNodeNum << "]";

    return "";
}

void Problem::problemController(CGPIndividual& individual, TYPE& fit) {
    cout << "Controller" << endl;
}

void Problem::problemSimulator() {
    cout << "Simulator" << endl;
}