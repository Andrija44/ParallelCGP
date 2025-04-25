#include "CGPIndividual.hpp"

using namespace std;
using namespace parallel_cgp;

CGPIndividual::CGPIndividual() {
    vector<vector<int>> branches;
    this->branches = branches;
    this->rows = 0;
    this->columns = 0;
    this->levelsBack = 0;
    this->inputs = 0;
    this->outputs = 0;
    this->evalDone = false;
}

CGPIndividual::CGPIndividual(vector<CGPNode> genes, vector<CGPOutput> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs) {
    vector<vector<int>> branches;
    this->branches = branches;
    this->genes = genes;
    this->outputGene = outputGene;
    this->rows = rows;
    this->columns = columns;
    this->levelsBack = levelsBack;
    this->inputs = inputs;
    this->outputs = outputs;
    this->evalDone = false;
}

CGPIndividual::CGPIndividual(vector<CGPNode> genes, vector<CGPOutput> outputGene, int rows, int columns, int levelsBack, int inputs, int outputs, bool evalDone) {
    vector<vector<int>> branches;
    this->branches = branches;
    this->genes = genes;
    this->outputGene = outputGene;
    this->rows = rows;
    this->columns = columns;
    this->levelsBack = levelsBack;
    this->inputs = inputs;
    this->outputs = outputs;
    this->evalDone = evalDone;
}

void CGPIndividual::printNodes() {
    for (size_t i = 0; i < rows * columns + inputs; i++)
        cout << i << " " << genes[i].operand << " " << genes[i].connection1 << " " << genes[i].connection2 << endl;

    for (size_t j = 0; j < outputs; j++)
        cout << outputGene[j].connection << " ";

    cout << endl << endl;
}

void CGPIndividual::evaluateUsed() {
    //#pragma omp parallel for
    for (int m = 0; m < outputs; m++)
        isUsed(outputGene[m].connection);

    evalDone = true;
}

void CGPIndividual::isUsed(int CGPNodeNum) {
    //#pragma omp atomic write
    genes[CGPNodeNum].used = true;

    //#pragma omp task
    if (genes[CGPNodeNum].connection1 >= 0)
        isUsed(genes[CGPNodeNum].connection1);

    //#pragma omp task
    if (genes[CGPNodeNum].connection2 >= 0)
        isUsed(genes[CGPNodeNum].connection2);

    //#pragma omp taskwait
}

void CGPIndividual::evaluateValue(vector<TYPE> input, function<TYPE(int, TYPE, TYPE)> &computeNode) {
    clearInd();

    for (int l = 0; l < inputs; l++)
        genes[l].outValue = input[l];

    //#pragma omp parallel
    //#pragma omp single
    for (int m = 0; m < outputs; m++)
        //#pragma omp task
        outputGene[m].value = evalNode(outputGene[m].connection, computeNode);
}

TYPE CGPIndividual::evalNode(int CGPNodeNum, function<TYPE(int, TYPE, TYPE)> &computeNode) {

    TYPE outVal;
    //#pragma omp atomic read
    outVal = genes[CGPNodeNum].outValue;
    if (!isnan(outVal))
        return outVal;

    TYPE value1, value2 = 0;

    //#pragma omp task shared(value1)
    value1 = evalNode(genes[CGPNodeNum].connection1, computeNode);
    if (genes[CGPNodeNum].connection2 >= 0)
        //#pragma omp task shared(value2)
        value2 = evalNode(genes[CGPNodeNum].connection2, computeNode);

    //#pragma omp taskwait

    TYPE newOutVal = computeNode(genes[CGPNodeNum].operand, value1, value2);
    //#pragma omp atomic write
    genes[CGPNodeNum].outValue = newOutVal;
    
    return genes[CGPNodeNum].outValue;
}

void CGPIndividual::clearInd() {
    //#pragma omp parallel for
    for (int i = inputs; i < genes.size(); i++)
        genes[i].outValue = NAN;
}

bool CGPIndividual::findLoops(int CGPNodeNum) {
    branches.clear();

    vector<int> CGPNodeSet;
    bool res;

    //#pragma omp parallel shared(res)
    //#pragma omp single
    res = loopFinder(CGPNodeNum, CGPNodeSet);

    return res;
}

bool CGPIndividual::loopFinder(int CGPNodeNum, vector<int> CGPNodeSet) {

    for (int i = 0; i < CGPNodeSet.size(); i++)
        if (CGPNodeSet[i] == CGPNodeNum) {
            CGPNodeSet.push_back(CGPNodeNum);
            //#pragma omp critical
            branches.push_back(CGPNodeSet);
            return true;
        }

    CGPNodeSet.push_back(CGPNodeNum);

    if (CGPNodeNum < inputs) {
        return false;
    }

    bool conn1, conn2;

    //#pragma omp task shared(conn1)
    conn1 = loopFinder(genes[CGPNodeNum].connection1, CGPNodeSet);
    //#pragma omp task shared(conn2)
    conn2 = genes[CGPNodeNum].connection2 == -1 ? false : loopFinder(genes[CGPNodeNum].connection2, CGPNodeSet);

    //#pragma omp taskwait
    return conn1 || conn2;
}

void CGPIndividual::resolveLoops() {

    Timer resLoopTime("resolveLoopsTimer");

    random_device rd;
    mt19937 gen(rd());

    for (int m = 0; m < outputs; m++) {
        while (findLoops(outputGene[m].connection)) {
            //#pragma omp parallel for firstprivate(gen) num_threads(omp_get_max_threads())
            for (int i = 0; i < branches.size(); i++) {
                uniform_int_distribution<> connectionDis(0, static_cast<int>(genes.size()) - 1);
                int cell1, cell2, con1, con2, con;

                #pragma omp atomic read
                cell1 = branches[i][branches[i].size() - 2];
                #pragma omp atomic read
                cell2 = branches[i][branches[i].size() - 1];

                #pragma omp atomic read
                con1 = genes[cell1].connection1;
                #pragma omp atomic read
                con2 = genes[cell1].connection2;

                if (con1 == cell2) {
                    while (true) {
                        #pragma omp atomic write
                        genes[cell1].connection1 = connectionDis(gen);

                        #pragma omp atomic read
                        con = genes[cell1].connection1;

                        if (con < inputs)
                            break;
                        if ((con % columns) == (cell1 % columns))
                            continue;
                        else if (((con - inputs) % columns) > (((cell1 - inputs) % columns) + levelsBack))
                            continue;
                        else
                            break;
                    }
                }
                else if (con2 == cell2) {
                    while (true) {
                        #pragma omp atomic write
                        genes[cell1].connection2 = connectionDis(gen);

                        #pragma omp atomic read
                        con = genes[cell1].connection2;
                        if (con < inputs)
                            break;
                        if ((con % columns) == (cell1 % columns))
                            continue;
                        else if (((con - inputs) % columns) > (((cell1 - inputs) % columns) + levelsBack))
                            continue;
                        else
                            break;
                    }
                }
            }
        }
    }

    resLoopTime.endTimer();
}