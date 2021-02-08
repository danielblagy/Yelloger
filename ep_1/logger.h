#pragma once

#include <stdio.h>
#include <mutex>


enum LogPriority
{
	TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
};

class Logger
{
private:
	static LogPriority priority;
	static std::mutex log_mutex;

public:
	static void set_priority(LogPriority new_priority)
	{
		priority = new_priority;
	}

	template<typename... Args>
	static void Trace(const char* message, Args... args)
	{
		if (priority <= TracePriority)
		{
			std::scoped_lock lock(log_mutex);
			printf("[Trace]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Debug(const char* message, Args... args)
	{
		if (priority <= DebugPriority)
		{
			std::scoped_lock lock(log_mutex);
			printf("[Debug]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Info(const char* message, Args... args)
	{
		if (priority <= InfoPriority)
		{
			std::scoped_lock lock(log_mutex);
			printf("[Info]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Warn(const char* message, Args... args)
	{
		if (priority <= WarnPriority)
		{
			std::scoped_lock lock(log_mutex);
			printf("[Warn]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Error(const char* message, Args... args)
	{
		if (priority <= ErrorPriority)
		{
			std::scoped_lock lock(log_mutex);
			printf("[Error]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template<typename... Args>
	static void Critical(const char* message, Args... args)
	{
		if (priority <= CriticalPriority)
		{
			std::scoped_lock lock(log_mutex);
			printf("[Critical]\t");
			printf(message, args...);
			printf("\n");
		}
	}
};

LogPriority Logger::priority = InfoPriority;
std::mutex Logger::log_mutex;