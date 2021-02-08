#include "logger.h"

#include <thread>


void log_test(int n)
{
	Logger::Info("%d", n);
}

int main()
{
	std::thread threads[10];

	for (int i = 0; i < 10; i++)
		threads[i] = std::thread(log_test, i);

	for (int i = 0; i < 10; i++)
		threads[i].join();

	return 0;
}