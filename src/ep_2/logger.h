#pragma once

#include <stdio.h>
#include <mutex>
#include <ctime>


enum LogPriority
{
	TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
};

class Logger
{
private:
	static LogPriority priority;
	static std::mutex log_mutex;
	static const char* filepath;
	static FILE* file;

public:
	static void SetPriority(LogPriority new_priority)
	{
		priority = new_priority;
	}

	static void EnableFileOutput()
	{
		filepath = "log.txt";
		enable_file_output();
	}

	static void EnableFileOutput(const char* new_filepath)
	{
		filepath = new_filepath;
		enable_file_output();
	}

	static void CloseFileOutput()
	{
		free_file();
	}

	template<typename... Args>
	static void Trace(const char* message, Args... args)
	{
		log("[Trace]\t", TracePriority, message, args...);
	}

	template<typename... Args>
	static void Debug(const char* message, Args... args)
	{
		log("[Debug]\t", DebugPriority, message, args...);
	}

	template<typename... Args>
	static void Info(const char* message, Args... args)
	{
		log("[Info]\t", InfoPriority, message, args...);
	}

	template<typename... Args>
	static void Warn(const char* message, Args... args)
	{
		log("[Warn]\t", WarnPriority, message, args...);
	}

	template<typename... Args>
	static void Error(const char* message, Args... args)
	{
		log("[Error]\t", ErrorPriority, message, args...);
	}

	template<typename... Args>
	static void Critical(const char* message, Args... args)
	{
		log("[Critical]\t", CriticalPriority, message, args...);
	}

private:
	template<typename... Args>
	static void log(const char* message_priority_str, LogPriority message_priority, const char* message, Args... args)
	{
		if (priority <= message_priority)
		{
			std::time_t current_time = std::time(0);
			std::tm* timestamp = std::localtime(&current_time);
			char buffer[80];
			strftime(buffer, 80, "%c", timestamp);

			std::scoped_lock lock(log_mutex);
			printf("%s\t", buffer);
			printf(message_priority_str);
			printf(message, args...);
			printf("\n");

			if (file)
			{
				fprintf(file, "%s\t", buffer);
				fprintf(file, message_priority_str);
				fprintf(file, message, args...);
				fprintf(file, "\n");
			}
		}
	}

	static void enable_file_output()
	{
		if (file != 0)
		{
			fclose(file);
		}

		file = fopen(filepath, "a");

		if (file == 0)
		{
			printf("Logger: Failed to open file at %s", filepath);
		}
	}

	static void free_file()
	{
		fclose(file);
		file = 0;
	}
};

LogPriority Logger::priority = InfoPriority;
std::mutex Logger::log_mutex;
const char* Logger::filepath = 0;
FILE* Logger::file = 0;