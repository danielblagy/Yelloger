#pragma once

#include "include/yelloger.h"

#include <assert.h>

#include <thread>


void log_test(int n)
{
	for (int i = 0; i < 100; i++)
	{
		Yellog::Info("%d: %d", n, i);
		Yellog::Warn("warning %d: %d", n, i);
	}

	for (int i = 0; i < 100000; i++)
	{
		int j = i * i;
	}

	for (int i = 0; i < 100; i++)
	{
		Yellog::Info("%d: %d", n, i);
		Yellog::Warn("warning %d: %d", n, i);
	}
}

void test_with_threads()
{
	std::thread threads[10];

	for (int i = 0; i < 10; i++)
		threads[i] = std::thread(log_test, i);

	for (int i = 0; i < 10; i++)
		threads[i].join();
}


int main()
{
	const char* name = "User";
	Yellog::Info("Hello %s", name);
	
	assert(Yellog::GetPriority() == Yellog::InfoPriority);
	
	Yellog::Info("Hello World!");
	
	assert(Yellog::GetFilepath() == NULL);
	assert(Yellog::IsFileOutputEnabled() == false);

	bool file_output_enabled = Yellog::EnableFileOutput();

	assert(file_output_enabled);

	assert(Yellog::GetFilepath() == "log.txt");
	assert(Yellog::IsFileOutputEnabled());

	Yellog::SetPriority(Yellog::DebugPriority);

	assert(Yellog::GetPriority() == Yellog::DebugPriority);

	assert(Yellog::GetTimestampFormat() == "%T  %d-%m-%Y");

	//Yellog::SetTimestampFormat("%c");
	//
	//assert(Yellog::GetTimestampFormat() == "%c");
	
	Yellog::Trace("shouldn't be logged");
	Yellog::Debug("debug log %s", "DEBUG! :)");
	Yellog::Info("log testing");
	Yellog::Critical("logger testing");

	//test_with_threads();
	
	return 0;
}