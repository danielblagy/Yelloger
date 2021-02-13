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
	DOCUMENTATION

	Ylgr doesn't need to be instantiated, just include the header and use it like this
		Ylgr::Info("Infotmation %d", int_value);
	, also there is no need to put newline character at the end of the message, it will be done automatically.


	To enable file output, call
		Ylgr::EnableFileOutput("mylogpath/mylog.txt");
	before using the logger.
	
	Optionally, you can provide no path
		Ylgr::EnableFileOutput();
	then, the logs will be saved in '/log.txt'.


	The default log priority is Ylgr::InfoPriority.
	You can set preority by calling
		Ylgr::SetPriority(Ylgr::DebugPriority);	// e.g. Ylgr::DebugPriority

	Possible values:
		Ylgr::TracePriority
		Ylgr::DebugPriority
		Ylgr::InfoPriority
		Ylgr::WarnPriority
		Ylgr::ErrorPriority
		Ylgr::CriticalPriority


	To log:
		Ylgr::Trace(const char* message, Args... args)
		Ylgr::Debug(const char* message, Args... args)
		Ylgr::Info(const char* message, Args... args)
		Ylgr::Warn(const char* message, Args... args)
		Ylgr::Error(const char* message, Args... args)
		Ylgr::Critical(const char* message, Args... args)
*/

#pragma once

#include <stdio.h>
#include <mutex>
#include <ctime>


class Ylgr
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
	FILE* file = 0;

public:
	// Set desired priority for the logger (messages with lower priority will not be recorded)
	// The default priority is Ylgr::InfoPriority
	static void SetPriority(LogPriority new_priority)
	{
		get_instance().priority = new_priority;
	}

	// Enable file output
	// Logs will be written to /log.txt
	// If the file doesn't exist, it will create it automatically
	// File will be closed when program stops
	static void EnableFileOutput()
	{
		Ylgr& logger_instance = get_instance();
		logger_instance.filepath = "log.txt";
		logger_instance.enable_file_output();
	}

	// Enable file output
	// Logs will be written to /filepath, provided the filepath is valid
	// If the file doesn't exist, it will create it automatically
	// File will be closed when program stops
	static void EnableFileOutput(const char* new_filepath)
	{
		Ylgr& logger_instance = get_instance();
		logger_instance.filepath = new_filepath;
		logger_instance.enable_file_output();
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Ylgr::TracePriority
	template<typename... Args>
	static void Trace(const char* message, Args... args)
	{
		get_instance().log("[Trace]\t", TracePriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Ylgr::DebugPriority
	template<typename... Args>
	static void Debug(const char* message, Args... args)
	{
		get_instance().log("[Debug]\t", DebugPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Ylgr::InfoPriority
	template<typename... Args>
	static void Info(const char* message, Args... args)
	{
		get_instance().log("[Info]\t", InfoPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Ylgr::WarnPriority
	template<typename... Args>
	static void Warn(const char* message, Args... args)
	{
		get_instance().log("[Warn]\t", WarnPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Ylgr::ErrorPriority
	template<typename... Args>
	static void Error(const char* message, Args... args)
	{
		get_instance().log("[Error]\t", ErrorPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Ylgr::CriticalPriority
	template<typename... Args>
	static void Critical(const char* message, Args... args)
	{
		get_instance().log("[Critical]\t", CriticalPriority, message, args...);
	}

private:
	Ylgr() {}

	Ylgr(const Ylgr&) = delete;
	Ylgr& operator= (const Ylgr&) = delete;

	~Ylgr()
	{
		free_file();
	}

	static Ylgr& get_instance()
	{
		static Ylgr instance;
		
		return instance;
	}

	template<typename... Args>
	void log(const char* message_priority_str, LogPriority message_priority, const char* message, Args... args)
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

	void enable_file_output()
	{
		if (file != 0)
		{
			fclose(file);
		}

		file = fopen(filepath, "a");

		if (file == 0)
		{
			printf("Ylgr: Failed to open file at %s\n", filepath);
		}
	}

	void free_file()
	{
		if (file)
		{
			fclose(file);
			file = 0;
		}
	}
};