#include "FuncProblem.h"

using namespace std;
using namespace parallel_cgp;

TYPE FuncProblem::computeNode(int operand, TYPE value1, TYPE value2) {
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

TYPE FuncProblem::fitness(TYPE x, TYPE y, TYPE res) {
    return func(x, y) - res;
}

void FuncProblem::printFunction() {
    if (isSimulated)
        cout << "Funkcija: " << evalFunction(bestI.outputGene[0].connection) << endl;
    else
        cout << "Problem nije simuliran." << endl;
}

string FuncProblem::evalFunction(int CGP1NodeNum) {
    ostringstream oss;

    if (CGP1NodeNum < INPUTS) {
        switch (CGP1NodeNum) {
        case 0:
            oss << "x";
            return oss.str();
        case 1:
            oss << "y";
            return oss.str();
        }
    }

    switch (bestI.genes[CGP1NodeNum].operand) {
    case 1:
        oss << "(" << evalFunction(bestI.genes[CGP1NodeNum].connection1) << " + " << evalFunction(bestI.genes[CGP1NodeNum].connection2) << ")";
        return oss.str();
    case 2:
        oss << "(" << evalFunction(bestI.genes[CGP1NodeNum].connection1) << " - " << evalFunction(bestI.genes[CGP1NodeNum].connection2) << ")";
        return oss.str();
    case 3:
        oss << "(" << evalFunction(bestI.genes[CGP1NodeNum].connection1) << " * " << evalFunction(bestI.genes[CGP1NodeNum].connection2) << ")";
        return oss.str();
    case 4:
        oss << "(" << evalFunction(bestI.genes[CGP1NodeNum].connection1) << " / " << evalFunction(bestI.genes[CGP1NodeNum].connection2) << ")";
        return oss.str();
    case 5:
        oss << "sin(" << evalFunction(bestI.genes[CGP1NodeNum].connection1) << ")";
        return oss.str();
    case 6:
        oss << "cos(" << evalFunction(bestI.genes[CGP1NodeNum].connection1) << ")";
        return oss.str();
    case 7:
        oss << "sqrt(" << evalFunction(bestI.genes[CGP1NodeNum].connection1) << ")";
        return oss.str();
    case 8:
        oss << evalFunction(bestI.genes[CGP1NodeNum].connection1) << "^2";
        return oss.str();
    case 9:
        oss << "2^" << evalFunction(bestI.genes[CGP1NodeNum].connection1);
        return oss.str();
    }

    return "";
}

void FuncProblem::problemController(CGPIndividual& individual, TYPE& fit) {
    function<TYPE(int op, TYPE v1, TYPE v2)> compNode =
        [&](int op, TYPE v1, TYPE v2) { return computeNode(op, v1, v2); };

    TYPE N = 0;

    for (TYPE x = -10; x < 10; x += 0.5) {
        for (TYPE y = -10; y < 10; y += 0.5) {
            vector<TYPE> input;
            input.push_back(x);
            input.push_back(y);

            individual.evaluateValue(input, compNode);
            fit += pow(fitness(x, y, individual.outputGene[0].value), 2);
            N++;
        }
    }

    fit /= N;
    fit = sqrt(fit);
}

void FuncProblem::problemSimulator() {
    CGP cgp(GENERATIONS, ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, MUTATIONS, NUM_OPERANDS, BI_OPERANDS, POPULATION_SIZE);

    vector<CGPIndividual> population;
    int bestInd = 0, generacija = 0;

    population = cgp.generatePopulation();

    for (generacija = 0; generacija < GENERATIONS; generacija++) {
        TYPE bestFit = 0;
        bestInd = 0;
        vector<int> bestInds;
        random_device rd;
        mt19937 gen(rd());

        for (int clan = 0; clan < POPULATION_SIZE; clan++) {

            TYPE fit = 0;
            problemController(population[clan], fit);

            if (clan == 0)
                bestFit = fit;

            if (fit < bestFit) {
                bestFit = fit;
                bestInds.clear();
                bestInds.push_back(clan);
            }
            else if (fit == bestFit)
                bestInds.push_back(clan);
        }

        if (bestInds.size() > 1)
            bestInds.erase(bestInds.begin());

        uniform_int_distribution<> bestDis(0, (int)bestInds.size() - 1);

        bestInd = bestInds[bestDis(gen)];

        cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << endl;

        if (bestFit <= 5)
            break;
        if (generacija != GENERATIONS - 1)
            population = cgp.goldMutate(population[bestInd]);
    }

    bestI = population[bestInd];

    isSimulated = true;

    printFunction();
}