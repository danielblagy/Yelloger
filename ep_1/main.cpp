#include "logger.h"

#include <thread>


void log_test(int n)
{
	Logger::Info("%d", n);
}

int main()
{
	Logger::EnableFileOutput("logs/log1.txt");
	
	std::thread threads[10];

	for (int i = 0; i < 10; i++)
		threads[i] = std::thread(log_test, i);

	for (int i = 0; i < 10; i++)
		threads[i].join();

	Logger::CloseFileOutput();

	return 0;
}