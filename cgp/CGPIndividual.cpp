#include "CGPIndividual.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

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
    for (int m = 0; m < outputs; m++)
        isUsed(outputGene[m].connection);

    evalDone = true;
}

void CGPIndividual::isUsed(int CGPNodeNum) {
    genes[CGPNodeNum].used = true;

    if (genes[CGPNodeNum].connection1 >= 0)
        isUsed(genes[CGPNodeNum].connection1);

    if (genes[CGPNodeNum].connection2 >= 0)
        isUsed(genes[CGPNodeNum].connection2);
}

void CGPIndividual::evaluateValue(vector<TYPE> input, function<TYPE(int, TYPE, TYPE)> computeNode) {
    clearInd();

    for (int l = 0; l < inputs; l++)
        genes[l].outValue = input[l];

    for (int m = 0; m < outputs; m++)
        outputGene[m].value = evalNode(outputGene[m].connection, computeNode);
}

TYPE CGPIndividual::evalNode(int CGPNodeNum, function<TYPE(int, TYPE, TYPE)> computeNode) {

    if (isnan(genes[CGPNodeNum].outValue)) {
        TYPE value1 = evalNode(genes[CGPNodeNum].connection1, computeNode);
        TYPE value2 = genes[CGPNodeNum].connection2 < 0 ? 0 : evalNode(genes[CGPNodeNum].connection2, computeNode);

        genes[CGPNodeNum].outValue = computeNode(genes[CGPNodeNum].operand, value1, value2);
    }
    
    return genes[CGPNodeNum].outValue;
}

void CGPIndividual::clearInd() {
    for (int i = inputs; i < genes.size(); i++)
        genes[i].outValue = NAN;
}

CGPIndividual CGPIndividual::deserialize(istream& is) {
    int rows, columns, levelsBack, inputs, outputs, evalDone;

    is >> rows >> columns >> levelsBack >> inputs >> outputs >> evalDone;

    size_t genesSize;
    is >> genesSize;
    vector<CGPNode> genes;
    genes.reserve(genesSize);
    for (size_t i = 0; i < genesSize; ++i) {
        CGPNode gene;
        is >> gene;
        genes.emplace_back(gene);
    }

    size_t outputGeneSize;
    is >> outputGeneSize;
    vector<CGPOutput> outputGene;
    outputGene.reserve(outputGeneSize);
    for (size_t i = 0; i < outputGeneSize; ++i) {
        CGPOutput outGene;
        is >> outGene;
        outputGene.emplace_back(outGene);
    }

    return CGPIndividual(genes, outputGene, rows, columns, levelsBack, inputs, outputs, evalDone);
}

bool CGPIndividual::findLoops(int CGPNodeNum, vector<int> CGPNodeSet) {
    branches.clear();

    return loopFinder(CGPNodeNum, CGPNodeSet);;
}

bool CGPIndividual::loopFinder(int CGPNodeNum, vector<int> CGPNodeSet) {

    for (int i = 0; i < CGPNodeSet.size(); i++)
        if (CGPNodeSet[i] == CGPNodeNum) {
            CGPNodeSet.push_back(CGPNodeNum);
            branches.push_back(CGPNodeSet);
            return true;
        }

    CGPNodeSet.push_back(CGPNodeNum);

    if (CGPNodeNum < inputs) {
        return false;
    }

    bool conn1 = loopFinder(genes[CGPNodeNum].connection1, CGPNodeSet);
    bool conn2 = genes[CGPNodeNum].connection2 == -1 ? false : loopFinder(genes[CGPNodeNum].connection2, CGPNodeSet);

    return conn1 || conn2;
}

void CGPIndividual::resolveLoops() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> connectionDis(0, static_cast<int>(genes.size()) - 1);

    vector<int> CGPNodeSet;

    for (int m = 0; m < outputs; m++) {
        while (findLoops(outputGene[m].connection, CGPNodeSet)) {
            for (int i = 0; i < branches.size(); i++) {
                int cell1 = branches[i][branches[i].size() - 2];
                int cell2 = branches[i][branches[i].size() - 1];

                if (genes[cell1].connection1 == cell2) {
                    genes[cell1].connection1 = connectionDis(gen);

                    while (true) {
                        if (genes[cell1].connection1 < inputs)
                            break;
                        if ((genes[cell1].connection1 % columns) == (cell1 % columns))
                            genes[cell1].connection1 = connectionDis(gen);
                        else if (((genes[cell1].connection1 - inputs) % columns) > (((cell1 - inputs) % columns) + levelsBack))
                            genes[cell1].connection1 = connectionDis(gen);
                        else
                            break;
                    }
                }
                else if (genes[cell1].connection2 == cell2) {
                    genes[cell1].connection2 = connectionDis(gen);

                    while (true) {
                        if (genes[cell1].connection2 < inputs)
                            break;
                        if ((genes[cell1].connection2 % columns) == (cell1 % columns))
                            genes[cell1].connection2 = connectionDis(gen);
                        else if (((genes[cell1].connection2 - inputs) % columns) > (((cell1 - inputs) % columns) + levelsBack))
                            genes[cell1].connection2 = connectionDis(gen);
                        else
                            break;
                    }
                }
            }

            CGPNodeSet.clear();
        }
    }
}