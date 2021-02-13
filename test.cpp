#pragma once

#include "include/yelloger.h"

#include <assert.h>

#include <thread>


void log_test(int n)
{
	for (int i = 0; i < 100; i++)
	{
		Ylgr::Info("%d: %d", n, i);
		Ylgr::Warn("warning %d: %d", n, i);
	}

	for (int i = 0; i < 100000; i++)
	{
		int j = i * i;
	}

	for (int i = 0; i < 100; i++)
	{
		Ylgr::Info("%d: %d", n, i);
		Ylgr::Warn("warning %d: %d", n, i);
	}
}


int main()
{
	assert(Ylgr::GetPriority() == Ylgr::InfoPriority);
	
	Ylgr::Info("Hello World!");
	
	Ylgr::EnableFileOutput();

	Ylgr::SetPriority(Ylgr::DebugPriority);

	assert(Ylgr::GetPriority() == Ylgr::DebugPriority);

	Ylgr::Trace("shouldn't be logged");
	Ylgr::Debug("debug log %s", "DEBUG! :)");
	Ylgr::Info("log testing");
	Ylgr::Critical("logger testing");

	std::thread threads[10];

	for (int i = 0; i < 10; i++)
		threads[i] = std::thread(log_test, i);

	for (int i = 0; i < 10; i++)
		threads[i].join();
	
	return 0;
}