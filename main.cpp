#include "Problem.hpp"
#include "Timer.hpp"
#include "boolProblem/BoolProblem.hpp"
#include "funcProblem/FuncProblem.hpp"
#include "waitProblem/WaitProblem.hpp"
#include "adProblem/ADProblem.hpp"

#include <iostream>
#include <omp.h>

using namespace std;
using namespace parallel_cgp;

int main() {
    int choice;
    cout << "Choose which problem to run" << endl << endl;
    cout << "1 - Boolean problem" << endl;
    cout << "2 - Parity problem" << endl;
    cout << "3 - Function problem" << endl;
    cout << "4 - Acey Deucey problem" << endl;
    cout << "5 - Wait problem" << endl;
    cout << endl << "Enter your choice: ";
    cin >> choice;

    Timer mainTime("mainTimer");

    Problem* problem = nullptr;

    if (choice == 1)
        problem = new BoolProblem;
    else if (choice == 2)
        problem = new ParityProblem;
    else if (choice == 3)
        problem = new FuncProblem;
    else if (choice == 4)
        problem = new ADProblem;
    else if (choice == 5)
        problem = new WaitProblem;
    else {
        cout << "Invalid option" << endl;
        return 1;
    }

    problem->problemRunner();
    delete(problem);

    mainTime.endTimer();
    Timer::printTimes();

    return 0;
}