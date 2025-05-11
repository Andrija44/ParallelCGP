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

int main(int ac, char** av) {

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("bool,b", "enable bool test")
            ("parity,p", "enable parity test")
            ("func,f", "enable func test")
            ("acey,a", "enable acey test")
            ("wait,w", "enable wait test")
            ("custom,c", po::value<std::vector<int>>()->multitoken(), "custom test values")
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

        if (vm.count("bool"))
            BoolTester boolTest = BoolTester(BoolParam(params[0], params[1], params[2], params[3], params[4]));
        if (vm.count("parity"))
            ParityTester parityTest = ParityTester(BoolParam(params[0], params[1], params[2], params[3], params[4]));
        if (vm.count("func"))
            FuncTester funcTest = FuncTester(FuncParam(params[0], params[1], params[2], params[3], params[4], -1));
        if (vm.count("acey"))
            ADTester adTest = ADTester(ADParam(params[0], params[1], params[2], params[3], params[4]));
        if (vm.count("wait"))
            WaitTester waitTest = WaitTester(WaitParam(params[0], params[1], params[2], params[3], params[4], 1));
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