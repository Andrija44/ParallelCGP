#include "CGP.hpp"

using namespace std;
using namespace parallel_cgp;

void CGP::generatePopulation(vector<CGPIndividual> &population) {
    // vrijeme za izvodenje cijele funkcije
    Timer genTime("generatePopulationTimer");

    boost::random::mt19937 gen(chrono::duration_cast<std::chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count() * (omp_get_thread_num() + 1));

    for (int i = 0; i < populationSize; i++) {
        boost::random::uniform_int_distribution<> operandDis(1, operands);
        boost::random::uniform_int_distribution<> connectionDis(0, rows * columns + inputs - 1);
        boost::random::uniform_int_distribution<> outputDis(0, rows * columns + inputs - 1);

        vector<CGPNode> genes;
        vector<CGPOutput> outputGene;

        for (int k = 0; k < inputs; k++) {
            CGPNode node;
            node.used = false;
            node.connection1 = -1;
            node.connection2 = -1;
            node.operand = -1;
            genes.push_back(node);
        }

        for (int j = inputs; j < rows * columns + inputs; j++) {
            CGPNode node;
            node.used = false;
            node.operand = operandDis(gen);
            node.connection1 = connectionDis(gen);
            node.outValue = NAN;

            while (true) {
                if (node.connection1 < inputs)
                    break;
                if ((node.connection1 % columns) == (j % columns))
                    node.connection1 = connectionDis(gen);
                else if (((node.connection1 - inputs) % columns) > (((j - inputs) % columns) + levelsBack))
                    node.connection1 = connectionDis(gen);
                else if(genes.size() > node.connection1 && (genes[node.connection1].connection1 == j || genes[node.connection1].connection2 == j))
                    node.connection1 = connectionDis(gen);
                else
                    break;
            }

            node.connection2 = (node.operand >= biOperands) ? -1 : connectionDis(gen);

            while (true) {
                if (node.connection2 < inputs)
                    break;
                if ((node.connection2 % columns) == (j % columns))
                    node.connection2 = connectionDis(gen);
                else if (((node.connection2 - inputs) % columns) > (((j - inputs) % columns) + levelsBack))
                    node.connection2 = connectionDis(gen);
                else if (genes.size() > node.connection2 && (genes[node.connection2].connection1 == j || genes[node.connection2].connection2 == j))
                    node.connection2 = connectionDis(gen);
                else
                    break;
            }
            genes.push_back(node);
        }

        for (int k = 0; k < outputs; k++) {
            CGPOutput output;

            output.connection = outputDis(gen);
            outputGene.push_back(output);
        }

        CGPIndividual individual(genes, outputGene, rows, columns, levelsBack, inputs, outputs);

        population[i] = individual;
        population[i].resolveLoops();
    }

    genTime.endTimer();
}

void CGP::goldMutate(CGPIndividual parent, vector<CGPIndividual> &population) {
    Timer mutTime("mutatePopulationTimer");

    if (!parent.evalDone)
        parent.evaluateUsed();
    population[0] = parent;

    boost::random::mt19937 gen(chrono::duration_cast<std::chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count() * (omp_get_thread_num() + 1));

    for (int n = 1; n < populationSize; n++) {
        boost::random::uniform_int_distribution<> nodDis(parent.inputs, static_cast<int>(parent.genes.size()));
        boost::random::uniform_int_distribution<> geneDis(0, 2);
        boost::random::uniform_int_distribution<> connectionDis(0, static_cast<int>(parent.genes.size()) - 1);
        boost::random::uniform_int_distribution<> operandDis(1, operands);
        boost::random::uniform_int_distribution<> outputDis(0, parent.outputs - 1);

        vector<CGPNode> genes = parent.genes;
        vector<CGPOutput> outputGene = parent.outputGene;
        bool isActive = false;

        while (!isActive) {
            int mut = geneDis(gen);
            int cell = nodDis(gen);
            if (cell == parent.genes.size()) {
                outputGene[outputDis(gen)].connection = connectionDis(gen);
                break;
            }
            if (mut == 0) {
                genes[cell].operand = operandDis(gen);
                
                if (genes[cell].operand >= biOperands && genes[cell].connection2 != -1)
                    genes[cell].connection2 = -1;
                else if (genes[cell].operand < biOperands && genes[cell].connection2 == -1)
                    genes[cell].connection2 = connectionDis(gen);
            }
            else if (mut == 1)
                genes[cell].connection1 = connectionDis(gen);
            else if (mut == 2 && genes[cell].operand >= biOperands)
                continue;
            else if (mut == 2)
                genes[cell].connection2 = connectionDis(gen);

            while (true) {
                if (genes[cell].connection1 < parent.inputs)
                    break;
                if ((genes[cell].connection1 % parent.columns) == (cell % parent.columns))
                    genes[cell].connection1 = connectionDis(gen);
                else if (((genes[cell].connection1 - parent.inputs) % parent.columns) > (((cell - parent.inputs) % parent.columns) + parent.levelsBack))
                    genes[cell].connection1 = connectionDis(gen);
                else
                    break;
            }

            while (true) {
                if (genes[cell].connection2 < parent.inputs)
                    break;
                if ((genes[cell].connection2 % parent.columns) == (cell % parent.columns))
                    genes[cell].connection2 = connectionDis(gen);
                else if (((genes[cell].connection2 - parent.inputs) % parent.columns) > (((cell - parent.inputs) % parent.columns) + parent.levelsBack))
                    genes[cell].connection2 = connectionDis(gen);
                else
                    break;
            }

            isActive = genes[cell].used;
        }

        for (size_t z = parent.inputs; z < genes.size(); z++)
            genes[z].used = false;

        CGPIndividual individual(genes, outputGene, parent.rows, parent.columns, parent.levelsBack, parent.inputs, parent.outputs);

        population[n] = individual;
        population[n].resolveLoops();
    }   

    mutTime.endTimer();
}