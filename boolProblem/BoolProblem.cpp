#include "BoolProblem.hpp"

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
        return ~value1;
    default:
        return 0;
    }
}

TYPE BoolProblem::fitness(bitset<INPUTS> in, TYPE res) {
    if (useFunc)
        return boolFunc(in) == res;

    return parityFunc(in) == res;
}

void BoolProblem::printFunction() {
    if (isSimulated)
        cout << "Funkcija: " << evalFunction(bestI->outputGene[0].connection) << endl;
    else
        cout << "Problem nije simuliran." << endl;
}

string BoolProblem::evalFunction(int CGPNodeNum) {
    ostringstream oss;

    if (CGPNodeNum < INPUTS) {
        oss << "bit[" << CGPNodeNum << "]";
        return oss.str();
    }

    switch (bestI->genes[CGPNodeNum].operand) {
    case 1:
        oss << "(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << " | " << evalFunction(bestI->genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 2:
        oss << "(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << " & " << evalFunction(bestI->genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 3:
        oss << "(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << " ^ " << evalFunction(bestI->genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 4:
        oss << "~" << evalFunction(bestI->genes[CGPNodeNum].connection1);
        return oss.str();
    }

    return "";
}

void BoolProblem::problemSimulator(CGPIndividual& individual, TYPE &fit) {
    function<double(int op, double v1, double v2)> compNode =
        [&](int op, double v1, double v2) { return computeNode(op, static_cast<TYPE>(v1), static_cast<TYPE>(v2)); };

    for (int perm = 0; perm < pow(2, INPUTS); ++perm) {
        bitset<INPUTS> bits(perm);
        vector<double> input;

        for (int i = 0; i < bits.size(); ++i)
            input.push_back(static_cast<double>(bits[i]));

        individual.evaluateValue(input, compNode);
        fit += fitness(bits, static_cast<int>(individual.outputGene[0].value));
    }
}

void BoolProblem::problemRunner() {
    CGP cgp(ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, NUM_OPERANDS, BI_OPERANDS, POPULATION_SIZE);

    vector<CGPIndividual> population(POPULATION_SIZE);
    int bestInd = 0, generacija = 0;

    cgp.generatePopulation(population);

    for (generacija = 0; generacija < GENERATIONS; generacija++) {
        TYPE bestFit = INT_MIN;
        bestInd = 0;
        vector<int> bestInds;
        random_device rd;
        mt19937 gen(rd());

        for (int clan = 0; clan < POPULATION_SIZE; clan++) {

            TYPE fit = 0;
            problemSimulator(population[clan], fit);

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
        if (bestInds.size() == 0)
            bestInds.push_back(0);

        uniform_int_distribution<> bestDis(0, static_cast<int>(bestInds.size() - 1));

        bestInd = bestInds[bestDis(gen)];

        cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << endl;

        if (bestFit == pow(2, INPUTS))
            break;
        if (generacija != GENERATIONS - 1)
            cgp.goldMutate(population[bestInd], population);
    }

    bestI = &population[bestInd];

    isSimulated = true;

    printFunction();
}