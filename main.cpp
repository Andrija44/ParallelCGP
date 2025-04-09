#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib> 
#include <ctime>
#include "BoolProblem.h"
#include "FuncProblem.h"

using namespace std;
using namespace parallel_cgp;

int main() {
    FuncProblem fp;

    fp.problemSimulator();

    return 0;
}