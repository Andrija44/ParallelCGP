#include "FuncProblem.hpp"

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
        cout << "Funkcija: " << evalFunction(bestI->outputGene[0].connection) << endl;
    else
        cout << "Problem nije simuliran." << endl;
}

string FuncProblem::evalFunction(int CGPNodeNum) {
    ostringstream oss;

    if (CGPNodeNum < INPUTS) {
        switch (CGPNodeNum) {
        case 0:
            oss << "x";
            return oss.str();
        case 1:
            oss << "y";
            return oss.str();
        }
    }

    switch (bestI->genes[CGPNodeNum].operand) {
    case 1:
        oss << "(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << " + " << evalFunction(bestI->genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 2:
        oss << "(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << " - " << evalFunction(bestI->genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 3:
        oss << "(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << " * " << evalFunction(bestI->genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 4:
        oss << "(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << " / " << evalFunction(bestI->genes[CGPNodeNum].connection2) << ")";
        return oss.str();
    case 5:
        oss << "sin(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << ")";
        return oss.str();
    case 6:
        oss << "cos(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << ")";
        return oss.str();
    case 7:
        oss << "sqrt(" << evalFunction(bestI->genes[CGPNodeNum].connection1) << ")";
        return oss.str();
    case 8:
        oss << evalFunction(bestI->genes[CGPNodeNum].connection1) << "^2";
        return oss.str();
    case 9:
        oss << "2^" << evalFunction(bestI->genes[CGPNodeNum].connection1);
        return oss.str();
    }

    return "";
}

void FuncProblem::problemSimulator(CGPIndividual& individual, TYPE& fit) {
    Timer probSimTime("problemSimulatorTimer");

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

    probSimTime.endTimer();
}

void FuncProblem::problemRunner() {
    Timer probRunTime("problemRunnerTimer");

    CGP cgp(ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, NUM_OPERANDS, BI_OPERANDS, POPULATION_SIZE);

    vector<CGPIndividual> population(POPULATION_SIZE);
    int bestInd = 0, generacija = 0;

    cgp.generatePopulation(population);

    for (generacija = 0; generacija < GENERATIONS; generacija++) {
        TYPE bestFit = DBL_MAX;
        bestInd = 0;
        vector<int> bestInds;
        boost::random::mt19937 gen(chrono::duration_cast<std::chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count() * (omp_get_thread_num() + 1));

        for (int clan = 0; clan < POPULATION_SIZE; clan++) {

            TYPE fit = 0;
            problemSimulator(population[clan], fit);

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
        if (bestInds.size() == 0)
            bestInds.push_back(0);

        boost::random::uniform_int_distribution<> bestDis(0, static_cast<int>(bestInds.size()) - 1);

        bestInd = bestInds[bestDis(gen)];

        if(printGens)
            cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << endl;

        if (bestFit <= THRESHOLD)
            break;
        if (generacija != GENERATIONS - 1)
            cgp.goldMutate(population[bestInd], population);
    }

    bestI = &population[bestInd];

    isSimulated = true;

    printFunction();

    probRunTime.endTimer();
}