# Yelloger
* [General Info](#motivation)
* [Reference](#reference-contents)
## A simple thread-safe single-header C++ 17 logger.
### Motivation
This logger was initially made for a [Youtube tutorial](https://youtube.com/playlist?list=PL5Lk2LPoiyAKcw7T-_FB_4BNrWkxfwnus).
### Include
This is a header-only library consisting of one header file. Simply copy the [include/yelloger.h](include/yelloger.h) file and `#include` it in your project.
### Simple Example
Console logging (the simplest use case)
```cpp
#include <yelloger.h>

int main()
{
	const char* name = "User";
	Yellog::Info("Hello %s", name);
	
	return 0;
}
```
Output:
> 15:07:31  15-02-2021    [Info]     Hello User


###  Quick Start
Yellog doesn't need to be instantiated, just include the header and use it like this
```cpp
	Yellog::Info("Infotmation %d", int_value);
```
, also there is no need to put newline character at the end of the message, it will be done automatically.


## Reference Contents
* [Log Priorities](#log-priorities)
* [Logging](#logging)
* [File Output](#file-output)
* [Timestamps](#timestamps)

## Reference

### Log Priorities
The default log priority is `Yellog::InfoPriority`. You can set priority by calling
```cpp
	Yellog::SetPriority(Yellog::DebugPriority);	// e.g. Yellog::DebugPriority
```

Possible values:
```cpp
	Yellog::TracePriority
	Yellog::DebugPriority
	Yellog::InfoPriority
	Yellog::WarnPriority
	Yellog::ErrorPriority
	Yellog::CriticalPriority
```
  
You can get priority by calling
```cpp
	Yellog::GetPriority();	// will return Yellog::InfoPriority if Yellog::SetPriority hasn't been called before
```


### Logging
To log:
```cpp
	Yellog::Trace(const char* message, Args... args)		// log a message with trace priority
	Yellog::Debug(const char* message, Args... args)		// log a message with debug priority
	Yellog::Info(const char* message, Args... args)			// log a message with info priority
	Yellog::Warn(const char* message, Args... args)			// log a message with warn priority
	Yellog::Error(const char* message, Args... args)		// log a message with error priority
	Yellog::Critical(const char* message, Args... args)		// log a message with critical priority
```

As args you can provide primitives and C-strings. Formatting follows [printf format](https://www.cplusplus.com/reference/cstdio/printf/).


### File Output
To enable file output, call
```cpp
	Yellog::EnableFileOutput("mylogpath/mylog.txt");
```
before using the logger.  
  
Optionally, you can provide no path
```cpp
	Yellog::EnableFileOutput();
```
then, the logs will be saved in '/log.txt'.  
  
To get the current filepath for file logging, call
```cpp
	Yellog::GetFilepath();
```
if file output was not enabled, the filepath will contain NULL, otherwise a const char* value will be returned.  
  
To check if file output was enabled and file was successfully opened, call
```cpp
	Yellog::IsFileOutputEnabled();	// returns true if success, false if failure
```


### Timestamps
Format follows ctime [strftime format specification](https://www.cplusplus.com/reference/ctime/strftime/).  
Default format is "%T  %d-%m-%Y" (e.g. 13:20:25  14-02-2021).  
4 spaces are added automatically to the end of timestamp each time the message is logged.  
  
To set a log timestamp format, call
```cpp
	Yellog::SetTimestampFormat("%c");	// e.g. Thu Aug 23 14:55:02 2001
```  
  
To get the current log timestamp format, call
```cpp
	Yellog::GetTimestampFormat();	// e.g. "13:20:25  14-02-2021"
```  
  
