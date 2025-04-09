#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib> 
#include <ctime>
#include "Problem.h"
#include "BoolProblem.h"

using namespace std;
using namespace parallel_cgp;

int main() {
    BoolProblem bp;

    bp.problemSimulator();

    return 0;
}