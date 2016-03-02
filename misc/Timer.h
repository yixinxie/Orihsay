#include <chrono>
namespace ori{
	class Timer{
	private:
		std::chrono::high_resolution_clock::time_point t0;
	public:
		Timer(void);
		long mark(void);
	};
}