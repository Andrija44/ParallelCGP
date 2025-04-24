#include "Problem.hpp"
#include "Timer.hpp"
#include "boolProblem/BoolTester.hpp"
#include "funcProblem/FuncTester.hpp"
#include "waitProblem/WaitTester.hpp"
#include "adProblem/ADTester.hpp"
#include "boolProblem/BoolProblem.hpp"
#include "funcProblem/FuncProblem.hpp"
#include "waitProblem/WaitProblem.hpp"
#include "adProblem/ADProblem.hpp"

#include <iostream>
#include <omp.h>

using namespace std;
using namespace parallel_cgp;

#define OMPSIM

#if (defined(_OPENMP) && (defined(OMPCGP) || defined(OMPSIM) || defined(OMPRUN)))
#define BoolTester ParBoolTester
#define ParityTester ParParityTester
#define FuncTester ParFuncTester
#define ADTester ParADTester
#define WaitTester ParWaitTester
#else
#define BoolTester SeqBoolTester
#define ParityTester SeqParityTester
#define FuncTester SeqFuncTester
#define ADTester SeqADTester
#define WaitTester SeqWaitTester
#endif

int main() {
    BoolTester boolTest;
    ParityTester parityTest;
    FuncTester funcTest;
    ADTester adTest;
    WaitTester waitTest;

    return 0;
}