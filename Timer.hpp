#ifndef TIMER_HPP
#define TIMER_HPP

#include <omp.h>
#include <chrono>
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>

#ifdef _OPENMP
#define timerFunc() omp_get_wtime()
#define timerDiff(startTime, endTime) (endTime - startTime)
#define TIME_UNIT double
#else
#define timerFunc() std::chrono::steady_clock::now()
#define timerDiff(startTime, endTime) (std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000000.0)
#define TIME_UNIT std::chrono::steady_clock::time_point
#endif

namespace parallel_cgp {

	class Timer
	{
	private:
		inline static std::map<std::string, std::vector<double>> mapa;

		std::string funcName;
		TIME_UNIT start;
		double end;
	public:
		Timer(std::string funcName) : funcName(funcName), start(timerFunc()), end(0) {}

		void endTimer() {
			end = timerDiff(start, timerFunc());

			#pragma omp critical
			parallel_cgp::Timer::mapa[funcName].push_back(end);
		}
	
		static void printTimes() {
			for (const auto& [key, value] : parallel_cgp::Timer::mapa)
				for (const auto& val : value)
					std::cout << '[' << key << "] = " << val << "; " << std::endl;
		}

		static void saveTimes(std::string filename, std::string testName, int gens, int rows, int cols, int levels, int pop) {
			std::ofstream myFile;
			myFile.open(filename, std::ios_base::app);
			myFile << "TEST NAME: " << testName;
			myFile << ", GENS: " << gens << ", ROWS: " << rows << ", COLUMNS: " << cols
				<< ", LEVELS BACK: " << levels << ", POP SIZE: " << pop << std::endl;

			for (const auto& [key, value] : parallel_cgp::Timer::mapa) {
				myFile << '[' << key << "],";
				for (const auto& val : value)
					myFile << val << ',';
				myFile << std::endl;
			}
			myFile.close();
		}

		static void clearTimes() {
			parallel_cgp::Timer::mapa.clear();
		}
	};
}

#endif

