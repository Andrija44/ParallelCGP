#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib> 
#include <ctime>
#include "Problem.h"
#include "BoolProblem.h"
#include "FuncProblem.h"

using namespace std;
using namespace parallel_cgp;

int main() {
    int choice;
    cout << "Choose which problem to run" << endl << endl;
    cout << "1 - Boolean problem" << endl;
    cout << "2 - Function problem" << endl;
    cout << endl << "Enter your choice: ";
    cin >> choice;

    Problem* problem = nullptr;

    if (choice == 1)
        problem = new BoolProblem;
    else if (choice == 2)
        problem = new FuncProblem;
    else
        cout << "Invalid option" << endl;

    problem->problemSimulator();

    return 0;
}