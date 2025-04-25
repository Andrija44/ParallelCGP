#include "WaitProblem.hpp"

using namespace std;
using namespace parallel_cgp;

TYPE WaitProblem::fitness(TYPE prev) {
	return ++prev;
}

void WaitProblem::printFunction() {
    if (isSimulated)
        cout << "Funkcija: " << evalFunction(0) << endl;
    else
        cout << "Problem nije simuliran." << endl;
}

string WaitProblem::evalFunction(int CGPNodeNum) {
    ostringstream oss;

    if (!CGPNodeNum) {
        oss << "Wait time: " << WAIT_TIME << "ms";
        return oss.str();
    }

    return "";
}

void WaitProblem::problemSimulator(CGPIndividual& individual, TYPE& fit) {
    Timer probSimTime("problemSimulatorTimer");

    function<TYPE(int op, TYPE v1, TYPE v2)> compNode =
        [&](int op, TYPE v1, TYPE v2) { return computeNode(op, v1, v2); };

    #pragma omp parallel for firstprivate(individual) shared(compNode) num_threads(omp_get_max_threads())
    for (int iter = 0; iter < 10; iter++) {
        vector<TYPE> input;
        input.push_back(iter);

        individual.evaluateValue(input, compNode);
        waitFunc();
    }
    fit = fitness(fit);

    probSimTime.endTimer();
}

void WaitProblem::problemRunner() {
    Timer probRunTime("problemRunnerTimer");

    CGP cgp(ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, NUM_OPERANDS, BI_OPERANDS, POPULATION_SIZE);

    vector<CGPIndividual> population(POPULATION_SIZE);
    int bestInd = 0, generacija = 0;

    cgp.generatePopulation(population);

    for (generacija = 0; generacija < GENERATIONS; generacija++) {
        TYPE bestFit = 0;
        bestInd = 0;
        vector<int> bestInds;
        random_device rd;
        mt19937 gen(rd());

        for (int clan = 0; clan < POPULATION_SIZE; clan++) {

            TYPE fit = generacija;
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

        uniform_int_distribution<> bestDis(0, static_cast<int>(bestInds.size()) - 1);

        bestInd = bestInds[bestDis(gen)];

        cout << "Gen: " << generacija << "; Fitness: " << bestFit << "; Indeks: " << bestInd << endl;

        if (generacija != GENERATIONS - 1)
            cgp.goldMutate(population[bestInd], population);
    }

    bestI = &population[bestInd];

    isSimulated = true;

    printFunction();

    probRunTime.endTimer();
}