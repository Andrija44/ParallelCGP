#ifndef TIMER_HPP
#define TIMER_HPP

#include <omp.h>
#include <chrono>
#include <map>
#include <string>
#include <functional>
#include <iostream>

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
		inline static std::map<std::string, double> mapa;

		std::string funcName;
		TIME_UNIT start;
		double end;
	public:
		Timer(std::string funcName) : funcName(funcName), start(timerFunc()), end(0) {
			#pragma omp critical
			{
				if (!parallel_cgp::Timer::mapa.count(funcName))
					parallel_cgp::Timer::mapa[funcName] = 0;
			}
		}

		void endTimer() {
			end = timerDiff(start, timerFunc());

			#pragma omp atomic update
			parallel_cgp::Timer::mapa[funcName] += end;
			
		}
	
		static void printTimes() {
			for (const auto& [key, value] : parallel_cgp::Timer::mapa)
				std::cout << '[' << key << "] = " << value << "; " << std::endl;
		}
	};
}

#endif

