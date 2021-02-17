/*
	MIT License

	Copyright (c) 2021 danielblagy
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

/*
	QUICK START

	Yellog doesn't need to be instantiated, just include the header and use it like this
		Yellog::Info("Infotmation %d", int_value);
	, also there is no need to put newline character at the end of the message, it will be done automatically.


	To enable file output, call
		Yellog::EnableFileOutput("mylogpath/mylog.txt");
	before using the logger.
	
	Optionally, you can provide no path
		Yellog::EnableFileOutput();
	then, the logs will be saved in '/log.txt'.


	The default log priority is Yellog::InfoPriority.
	
	You can set priority by calling
		Yellog::SetPriority(Yellog::DebugPriority);	// e.g. Yellog::DebugPriority

	Possible values:
		Yellog::TracePriority
		Yellog::DebugPriority
		Yellog::InfoPriority
		Yellog::WarnPriority
		Yellog::ErrorPriority
		Yellog::CriticalPriority

	You can get priority by calling
		Yellog::GetPriority();	// will return Yellog::InfoPriority if Yellog::SetPriority hasn't been called before


	To log:
		Yellog::Trace(const char* message, Args... args)		// log a message with trace priority
		Yellog::Debug(const char* message, Args... args)		// log a message with debug priority
		Yellog::Info(const char* message, Args... args)		// log a message with info priority
		Yellog::Warn(const char* message, Args... args)		// log a message with warn priority
		Yellog::Error(const char* message, Args... args)		// log a message with error priority
		Yellog::Critical(const char* message, Args... args)	// log a message with critical priority

		As args you can provide primitives and C-strings. Formatting follows printf format.

	SIMPLE EXAMPLE

	#include <yelloger.h>

	int main()
	{
		const char* name = "User";
		Yellog::Info("Hello %s", name);
	
		return 0;
	}

	Output:
	15:07:31  15-02-2021    [Info]        Hello User
*/

#pragma once

// if msvc compiler is used, define _CRT_SECURE_NO_WARNINGS
#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <mutex>
#include <ctime>


class Yellog
{
public:
	enum LogPriority
	{
		TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
	};

private:
	LogPriority priority = InfoPriority;
	std::mutex log_mutex;
	
	const char* filepath = 0;
	std::FILE* file = 0;
	
	// for timestamp formatting
	char buffer[80];
	const char* timestamp_format = "%T  %d-%m-%Y";

public:
	// Set desired priority for the logger (messages with lower priority will not be recorded)
	// The default priority is Yellog::InfoPriority
	static void SetPriority(LogPriority new_priority)
	{
		get_instance().priority = new_priority;
	}

	// Get the current logger priority (messages with lower priority will not be recorded)
	// The default priority is Yellog::InfoPriority
	static LogPriority GetPriority()
	{
		return get_instance().priority;
	}

	// Enable file output
	// Logs will be written to /log.txt
	// If the file doesn't exist, it will create it automatically
	// File will be closed when program stops
	// Returns true if a file was successfully opened, false otherwise
	static bool EnableFileOutput()
	{
		Yellog& logger_instance = get_instance();
		logger_instance.filepath = "log.txt";
		return logger_instance.enable_file_output();
	}

	// Enable file output
	// Logs will be written to /filepath, provided the filepath is valid
	// If the file doesn't exist, it will create it automatically
	// File will be closed when program stops
	// Returns true if a file was successfully opened, false otherwise
	static bool EnableFileOutput(const char* new_filepath)
	{
		Yellog& logger_instance = get_instance();
		logger_instance.filepath = new_filepath;
		return logger_instance.enable_file_output();
	}

	// Returns the current filepath for file logging
	// if Yellog::EnableFileOutput was called without specifying a filepath, the filepath will be "log.txt"
	// if file output was not enabled, the filepath will contain NULL
	static const char* GetFilepath()
	{
		return get_instance().filepath;
	}

	// Returns true is file output was enabled and file was successfully opened, false if it wasn't
	static bool IsFileOutputEnabled()
	{
		return get_instance().file != 0;
	}

	// Set a log timestamp format
	// Format follows <ctime> strftime format specification
	// Default format is "%T  %d-%m-%Y" (e.g. 13:20:25  14-02-2021)
	// 4 spaces are added automatically to the end of timestamp each time the message is logged
	static void SetTimestampFormat(const char* new_timestamp_format)
	{
		get_instance().timestamp_format = new_timestamp_format;
	}

	// Get the current log timestamp format
	// Format follows <ctime> strftime format specification
	// Default format is "%T  %d-%m-%Y" (e.g. 13:20:25  14-02-2021)
	static const char* GetTimestampFormat()
	{
		return get_instance().timestamp_format;
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::TracePriority
	template<typename... Args>
	static void Trace(const char* message, Args... args)
	{
		get_instance().log("[Trace]    ", TracePriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::DebugPriority
	template<typename... Args>
	static void Debug(const char* message, Args... args)
	{
		get_instance().log("[Debug]    ", DebugPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::InfoPriority
	template<typename... Args>
	static void Info(const char* message, Args... args)
	{
		get_instance().log("[Info]     ", InfoPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::WarnPriority
	template<typename... Args>
	static void Warn(const char* message, Args... args)
	{
		get_instance().log("[Warn]     ", WarnPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::ErrorPriority
	template<typename... Args>
	static void Error(const char* message, Args... args)
	{
		get_instance().log("[Error]    ", ErrorPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::CriticalPriority
	template<typename... Args>
	static void Critical(const char* message, Args... args)
	{
		get_instance().log("[Crit]     ", CriticalPriority, message, args...);
	}

private:
	Yellog() {}

	Yellog(const Yellog&) = delete;
	Yellog& operator= (const Yellog&) = delete;

	~Yellog()
	{
		free_file();
	}

	static Yellog& get_instance()
	{
		static Yellog instance;
		
		return instance;
	}

	template<typename... Args>
	void log(const char* message_priority_str, LogPriority message_priority, const char* message, Args... args)
	{
		if (priority <= message_priority)
		{
			std::time_t current_time = std::time(0);
			std::tm* timestamp = std::localtime(&current_time);

			std::scoped_lock lock(log_mutex);
			std::strftime(buffer, 80, timestamp_format, timestamp);
			std::printf("%s    ", buffer);
			std::printf(message_priority_str);
			std::printf(message, args...);
			std::printf("\n");

			if (file)
			{
				std::fprintf(file, "%s    ", buffer);
				std::fprintf(file, message_priority_str);
				std::fprintf(file, message, args...);
				std::fprintf(file, "\n");
			}
		}
	}

	bool enable_file_output()
	{
		free_file();

		file = std::fopen(filepath, "a");

		if (file == 0)
		{
			return false;
		}

		return true;
	}

	void free_file()
	{
		if (file)
		{
			std::fclose(file);
			file = 0;
		}
	}
};