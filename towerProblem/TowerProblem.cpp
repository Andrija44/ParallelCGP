#include "TowerProblem.h"

using namespace std;
using namespace parallel_cgp;

TYPE TowerProblem::computeNode(int operand, TYPE value1, TYPE value2) {
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

TYPE TowerProblem::fitness(TYPE res, bool illegalMove, bool reachedEnd) {
    TYPE fit = res - towerFunc();
    if (fit < 0) fit = 0;

    if (illegalMove) fit += towerFunc() * 20;
    if (reachedEnd) fit -= towerFunc() / 5;

    if (fit < 0) fit = 0;

    return fit;
}

void TowerProblem::printFunction() {
    if (isSimulated) {
        for (int i = 0; i < OUTPUTS; i++)
            cout << "Funkcija " << i << ": " << evalFunction(bestI.outputGene[i].connection) << endl;
    }
    else
        cout << "Problem nije simuliran." << endl;
}

string TowerProblem::evalFunction(int CGP1NodeNum) {
    ostringstream oss;

    if (CGP1NodeNum < INPUTS) {
        switch (CGP1NodeNum) {
        case 0:
            oss << "t1";
            return oss.str();
        case 1:
            oss << "t2";
            return oss.str();
        case 2:
            oss << "t3";
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

bool TowerProblem::testMove(vector<bitset<DISK_NUM>>& bits, int from, int to) {
    int smallestFrom, smallestTo;

    for (smallestFrom = 0; smallestFrom < DISK_NUM; smallestFrom++)
        if (bits[from][smallestFrom]) break;

    for (smallestTo = 0; smallestTo < DISK_NUM; smallestTo++)
        if (bits[to][smallestTo]) break;

    if (smallestFrom >= smallestTo)
        return true;

    bits[from][smallestFrom].flip();
    bits[to][smallestFrom].flip();

    return false;
}

void TowerProblem::problemSimulator(CGPIndividual& individual, TYPE& fit) {
    function<double(int op, double v1, double v2)> compNode =
        [&](int op, double v1, double v2) { return computeNode(op, static_cast<TYPE>(v1), static_cast<TYPE>(v2)); };
    
    vector<bitset<DISK_NUM>> bits;

    bitset<DISK_NUM> bit(towerFunc());
    bits.push_back(bit);

    for (int i = 1; i < TOWER_NUM; i++) {
        bitset<DISK_NUM> bit0(0);
        bits.push_back(bit0);
    }

    bool illegalMove = false;
    bool reachedEnd = false;
    long moveCount = 0;

    while (!illegalMove && !reachedEnd && moveCount < towerFunc() * 10) {
        vector<double> input;

        for (int i = 0; i < TOWER_NUM; i++)
            input.push_back(static_cast<TYPE>(bits[i].to_ulong()));

        individual.evaluateValue(input, compNode);

        int from = static_cast<TYPE>(individual.outputGene[0].value);
        int to = static_cast<TYPE>(individual.outputGene[1].value);

        if (from < 0 || from > TOWER_NUM - 1 || to < 0 || to > TOWER_NUM - 1) {
            illegalMove = true;
            continue;
        }

        illegalMove = testMove(bits, from, to);
        reachedEnd = (bits[TOWER_NUM - 1].count() == DISK_NUM);
        moveCount++;
    }

    fit = fitness(moveCount, illegalMove, reachedEnd);
}

void TowerProblem::problemRunner() {
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
            problemSimulator(population[clan], fit);

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

        uniform_int_distribution<> bestDis(0, static_cast<int>(bestInds.size()) - 1);

        bestInd = bestInds[bestDis(gen)];

        cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << endl;

        if (bestFit == 0)
            break;
        if (generacija != GENERATIONS - 1)
            population = cgp.goldMutate(population[bestInd]);
    }

    bestI = population[bestInd];

    isSimulated = true;

    printFunction();
}