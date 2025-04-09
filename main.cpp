#include <iostream>
#include "Problem.h"
#include "boolProblem/BoolProblem.h"
#include "funcProblem/FuncProblem.h"
#include "waitProblem/WaitProblem.h"

using namespace std;
using namespace parallel_cgp;

int main() {
    int choice;
    cout << "Choose which problem to run" << endl << endl;
    cout << "1 - Boolean problem" << endl;
    cout << "2 - Function problem" << endl;
    cout << "3 - Tower problem" << endl;
    cout << "4 - Wait problem" << endl;
    cout << endl << "Enter your choice: ";
    cin >> choice;

    Problem* problem = nullptr;

    if (choice == 1)
        problem = new BoolProblem;
    else if (choice == 2)
        problem = new FuncProblem;
    else if (choice == 3)
        return 0;
    else if (choice == 4)
        problem = new WaitProblem;
    else
        cout << "Invalid option" << endl;

    problem->problemSimulator();

    return 0;
}