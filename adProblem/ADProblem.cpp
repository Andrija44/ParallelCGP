#include "ADProblem.hpp"

using namespace std;
using namespace parallel_cgp;

TYPE ADProblem::computeNode(int operand, TYPE value1, TYPE value2) {
    switch (operand) {
    case 1:
        return value1 + value2;
    case 2:
        return value1 - value2;
    case 3:
        return value1 * value2;
    case 4:
        return -value1;
    default:
        return 0;
    }
}

double ADProblem::fitness(TYPE cash, TYPE maxCash, double avgCash) {
    double score = avgCash;

    if (maxCash >= STARTING_CASH * 2)
        score += 50;
    if (cash <= 0)
        score -= 100;
    if (maxCash == MAX_CASH)
        score += 150;

    return score;
}

void ADProblem::printFunction() {
    if (isSimulated)
        cout << "Funkcija: " << evalFunction(bestI->outputGene[0].connection) << endl;
    else
        cout << "Problem nije simuliran." << endl;
}

string ADProblem::evalFunction(int CGPNodeNum) {
    ostringstream oss;

    if (CGPNodeNum < INPUTS) {
        switch (CGPNodeNum) {
        case 0:
            oss << "card1";
            return oss.str();
        case 1:
            oss << "card2";
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
        oss << "-" << evalFunction(bestI->genes[CGPNodeNum].connection1);
        return oss.str();
    }

    return "";
}

void ADProblem::problemSimulator(CGPIndividual& individual, double& fit) {
    function<double(int op, double v1, double v2)> compNode =
        [&](int op, double v1, double v2) { return computeNode(op, static_cast<TYPE>(v1), static_cast<TYPE>(v2)); };

    int card, win;
    int cash = STARTING_CASH, maxCash = STARTING_CASH;
    double avgCash = 0;

    for (int i = 0; i < CARD_SETS; i++) {
        card = static_cast<int>(sets[i].back());

        if (card > sets[i].at(0) && card < sets[i].at(1))
            win = 1;
        else if (card == sets[i].at(0) || card == sets[i].at(1))
            win = -1;
        else
            win = 0;

        individual.evaluateValue(sets[i], compNode);

        if (individual.outputGene[0].value > 1) {
            if (win == 1)
                cash += 10;
            else if (win == 0)
                cash -= 10;
            else if (win == -1)
                cash -= 20;
        }

        if (cash > maxCash)
            maxCash = cash;

        avgCash += cash;
    }

    avgCash /= static_cast<double>(CARD_SETS);
    fit = fitness(cash, maxCash, avgCash);
}

void ADProblem::problemRunner() {
    CGP cgp(ROWS, COLUMNS, LEVELS_BACK, INPUTS, OUTPUTS, NUM_OPERANDS, BI_OPERANDS, POPULATION_SIZE);

    vector<CGPIndividual> population(POPULATION_SIZE);
    int bestInd = 0, generacija = 0;

    cgp.generatePopulation(population);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> cardDis(1, 13);

    for (int j = 0; j < CARD_SETS; j++) {
        vector<double> set;
        for (int i = 0; i < 3; i++)
            set.push_back(static_cast<double>(cardDis(gen)));

        double card = set.back();
        set.pop_back();
        sort(set.begin(), set.end());
        set.push_back(card);

        sets.push_back(set);
    }

    for (generacija = 0; generacija < GENERATIONS; generacija++) {
        double bestFit = DBL_MIN;
        bestInd = 0;
        vector<int> bestInds;
        random_device rd;
        mt19937 gen(rd());

        for (int clan = 0; clan < POPULATION_SIZE; clan++) {

            double fit = 0;
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

        if (bestFit >= STARTING_CASH * 3)
            break;
        if (generacija != GENERATIONS - 1)
            cgp.goldMutate(population[bestInd], population);
    }

    bestI = &population[bestInd];

    isSimulated = true;

    printFunction();

    playGame();
}

void ADProblem::playGame() {
    function<double(int op, double v1, double v2)> compNode =
        [&](int op, double v1, double v2) { return computeNode(op, static_cast<TYPE>(v1), static_cast<TYPE>(v2)); };

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> cardDis(1, 13);

    int steps = 0;
    int cash = STARTING_CASH, maxCash = STARTING_CASH;

    while (cash && steps < 100 && cash < MAX_CASH) {
        vector<double> input;
        int card, win;
        for (int i = 0; i < 3; i++)
            input.push_back(static_cast<TYPE>(cardDis(gen)));

        card = card = static_cast<int>(input.back());
        input.pop_back();

        sort(input.begin(), input.end());

        if (card > input.at(0) && card < input.at(1))
            win = 1;
        else if (card == input.at(0) || card == input.at(1))
            win = -1;
        else
            win = 0;

        bestI->evaluateValue(input, compNode);

        cout << "Cash: " << cash << "; Cards: " << input[0] << ", " << input[1] << "; Bet: " << ((bestI->outputGene[0].value > 1) ? "YES" : "NO")
            << "; Third card: " << card << ((win == 1) ? " | WIN!" : " | LOST!") << endl;

        if (bestI->outputGene[0].value > 1) {
            if (win == 1)
                cash += 10;
            else if (win == 0)
                cash -= 10;
            else if (win == -1)
                cash -= 20;
        }

        if (cash > maxCash)
            maxCash = cash;

        steps++;
    }
}