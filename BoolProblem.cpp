#include "BoolProblem.h"

using namespace std;
using namespace parallel_cgp;

TYPE BoolProblem::computeNode(int operand, TYPE value1, TYPE value2) {
    switch (operand) {
    case 1:
        return value1 | value2;
    case 2:
        return value1 & value2;
    case 3:
        return value1 ^ value2;
    case 4:
        return value1 << value2;
    case 5:
        return value1 >> value2;
    case 6:
        return ~value1;
    default:
        return 0;
    }
}

TYPE BoolProblem::fitness(bitset<INPUTS> in, int res) {
    return boolFunc(in) == res;
}

void BoolProblem::printFunction() {
    cout << "Funkcija: " << evalFunction(bestI.outputGene[0].connection) << endl;
}

std::string BoolProblem::evalFunction(int CGPNodeNum) {
    std::ostringstream oss;

    if (CGPNodeNum < INPUTS) {
        oss << "bit[" << CGPNodeNum << "]";
        return oss.str();
    }

    switch (bestI.genes[CGPNodeNum].operand) {
    case 1:
        oss << "(" << evalFunction(bestI.genes[CGPNodeNum].connection1) << " | " << evalFunction(bestI.genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 2:
        oss << "(" << evalFunction(bestI.genes[CGPNodeNum].connection1) << " & " << evalFunction(bestI.genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 3:
        oss << "(" << evalFunction(bestI.genes[CGPNodeNum].connection1) << " ^ " << evalFunction(bestI.genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 4:
        oss << "(" << evalFunction(bestI.genes[CGPNodeNum].connection1) << " << " << evalFunction(bestI.genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 5:
        oss << "(" << evalFunction(bestI.genes[CGPNodeNum].connection1) << " >> " << evalFunction(bestI.genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 6:
        oss << "~" << evalFunction(bestI.genes[CGPNodeNum].connection1);
        return oss.str();
    }

    return "";
}

void BoolProblem::problemController(CGPIndividual& individual, TYPE &fit) {
    function<double(int op, double v1, double v2)> compNode =
        [&](int op, double v1, double v2) { return computeNode(op, (int)v1, (int)v2); };

    for (int perm = 0; perm < pow(2, INPUTS); ++perm) {
        bitset<INPUTS> bits(perm);
        vector<double> input;

        for (int i = 0; i < bits.size(); ++i)
            input.push_back((double)bits[i]);

        individual.evaluateValue(input, compNode);
        fit += fitness(bits, (int) individual.outputGene[0].value);
    }
}

void BoolProblem::problemSimulator() {
    CGP cgp(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS, NUM_OPERANDS, BI_OPERANDS, POPULATION_SIZE);
    CGPIndividual ind;

    vector<CGPIndividual> population;
    int bestInd = 0, generacija = 0, brEvaluacija = 0;

    population = cgp.generatePopulation();

    for (generacija = 0; generacija < GENERATIONS; generacija++) {
        TYPE bestFit = -1;
        bestInd = 0;
        vector<int> bestInds;
        random_device rd;
        mt19937 gen(rd());

        for (int clan = 0; clan < POPULATION_SIZE; clan++) {

            int fit = 0;
            problemController(population[clan], fit);

            if (fit > bestFit) {
                bestFit = fit;
                bestInds.clear();
                bestInds.push_back(clan);
            }
            else if (fit == bestFit)
                bestInds.push_back(clan);
        }

        if (bestInds.size() > 1)
            bestInds.erase(bestInds.begin());

        uniform_int_distribution<> bestDis(0, (int) bestInds.size() - 1);

        bestInd = bestInds[bestDis(gen)];

        std::cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << endl;

        if (bestFit == pow(2, INPUTS))
            break;
        if (generacija != GENERATIONS - 1)
            population = cgp.goldMutate(population[bestInd]);
    }

    //try {
    //    ofstream outFile(bestFile);
    //    if (outFile.is_open()) {
    //        outFile << population[bestInd];
    //        outFile.close();
    //        std::cout << "CGP written to text file." << endl;
    //    }
    //}
    //catch (const exception& e) {
    //    cerr << "Error writing CGP: " << e.what() << endl;
    //}

    bestI = population[bestInd];

    printFunction();
}