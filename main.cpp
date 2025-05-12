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
#include <boost/program_options.hpp>

#define PARAM_COUNT 5

using namespace std;
using namespace parallel_cgp;
namespace po = boost::program_options;

#define OMPRUN

#if (defined(_OPENMP) && (defined(OMPCGP) || defined(OMPSIM) || defined(OMPRUN)))
#define BoolTester ParBoolTester
#define ParityTester ParParityTester
#define FuncTester ParFuncTester
#define ADTester ParADTester
#define WaitTester ParWaitTester
#define PARALLEL_TESTER 1
#else
#define BoolTester SeqBoolTester
#define ParityTester SeqParityTester
#define FuncTester SeqFuncTester
#define ADTester SeqADTester
#define WaitTester SeqWaitTester
#define PARALLEL_TESTER 0
#endif

int main(int ac, char** av) {
    try {
        Tester::VERSION_NAME.assign("Run");
        int threads = 1;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("test,t", "enable testing")
            ("bool,b", "enable bool problem")
            ("parity,p", "enable parity problem")
            ("func,f", "enable func problem")
            ("acey,a", "enable acey problem")
            ("wait,w", "enable wait problem")
            ("custom,c", po::value<std::vector<int>>()->multitoken(), "custom test values (number of generations, rows, columns, levels, population size)")
            ("threads,T", po::value<int>(), "number of threads to use in parallel version")
            ("version,v", "print version information")
            ;
    
        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        vector<int> params(PARAM_COUNT, 0);
        if (!vm["custom"].empty() && !((params = vm["custom"].as<vector<int> >()).size() == PARAM_COUNT))
            throw invalid_argument("Not the right amount of custom parameters");
    
        if (vm.count("help")) {
            cout << desc << endl;
            return 1;
        }

        if (vm.count("version")) {
            cout << "ParallelCGP version 1.0 OpenMP Runner" << endl;
            cout << "Author: Andrija Macek" << endl;
            return 2;
        }

        if (vm.count("threads")) {
            if (!PARALLEL_TESTER)
                throw invalid_argument("Threads are not supported in the sequential version of the program");
            threads = vm["threads"].as<int>();
            if (threads < 1)
                throw invalid_argument("Number of threads must be greater than 0");
            Tester::threadNums.clear();
            Tester::threadNums.push_back(threads);
        }

        Problem* problem = nullptr;

        if (vm.count("bool"))
            if (vm.count("test"))
                BoolTester boolTest = BoolTester(BoolParam(params[0], params[1], params[2], params[3], params[4]));
            else {
                omp_set_num_threads(threads);
                problem = new BoolProblem;
                problem->printGens = true;
                problem->problemRunner();
            }
        if (vm.count("parity"))
            if (vm.count("test"))
                ParityTester parityTest = ParityTester(BoolParam(params[0], params[1], params[2], params[3], params[4]));
            else {
                omp_set_num_threads(threads);
                problem = new ParityProblem;
                problem->printGens = true;
                problem->problemRunner();
            }
        if (vm.count("func"))
            if (vm.count("test"))
                FuncTester funcTest = FuncTester(FuncParam(params[0], params[1], params[2], params[3], params[4], -1));
            else {
                omp_set_num_threads(threads);
                problem = new FuncProblem;
                problem->printGens = true;
                problem->problemRunner();
            }
        if (vm.count("acey"))
            if (vm.count("test"))
                ADTester adTest = ADTester(ADParam(params[0], params[1], params[2], params[3], params[4]));
            else {
                omp_set_num_threads(threads);
                problem = new ADProblem;
                problem->printGens = true;
                problem->problemRunner();
            }
        if (vm.count("wait"))
            if (vm.count("test"))
                WaitTester waitTest = WaitTester(WaitParam(params[0], params[1], params[2], params[3], params[4], 1));
            else {
                omp_set_num_threads(threads);
                problem = new WaitProblem;
                problem->printGens = true;
                problem->problemRunner();
            }

        if (vm.count("test"))
            delete(problem);
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << endl;
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!" << endl;
    }

    return 0;
}