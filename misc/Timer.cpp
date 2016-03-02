#include "Timer.h"
ori::Timer::Timer(){
	t0 = std::chrono::high_resolution_clock::now();
}
long ori::Timer::mark(){
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	long duration = (t1 - t0).count();
	t0 = t1;
	return duration;
}