# Yelloger
## A simple thread-safe single-header C++ 17 logger.
### Tut
### Include
This is a header-only ibrary consisting of one header file. Simply copy the include/yelloger.h file
(in the root of this repo) and #include it in your project.
### Examples
Console logging (the simplest use case)
```cpp
#include <yelloger.h>

int main()
{
	const char* name = "User";
	Ylgr::Info("Hello %s", name);
	
	return 0;
}
```
Output:
```
15:07:31  15-02-2021    [Info]        Hello User
```
###  Documentation
Ylgr doesn't need to be instantiated, just include the header and use it like this
```cpp
	Ylgr::Info("Infotmation %d", int_value);
```
, also there is no need to put newline character at the end of the message, it will be done automatically.

###  
To enable file output, call
```cpp
	Ylgr::EnableFileOutput("mylogpath/mylog.txt");
```
before using the logger.

Optionally, you can provide no path
```cpp
	Ylgr::EnableFileOutput();
```
then, the logs will be saved in '/log.txt'.

### 
The default log priority is Ylgr::InfoPriority.

You can set priority by calling
```cpp
	Ylgr::SetPriority(Ylgr::DebugPriority);	// e.g. Ylgr::DebugPriority
```

Possible values:
```cpp
	Ylgr::TracePriority
	Ylgr::DebugPriority
	Ylgr::InfoPriority
	Ylgr::WarnPriority
	Ylgr::ErrorPriority
	Ylgr::CriticalPriority
```

You can get priority by calling
```cpp
	Ylgr::GetPriority();	// will return Ylgr::InfoPriority if Ylgr::SetPriority hasn't been called before
```


To log:
```cpp
	Ylgr::Trace(const char* message, Args... args)		// log a message with trace priority
	Ylgr::Debug(const char* message, Args... args)		// log a message with debug priority
	Ylgr::Info(const char* message, Args... args)		// log a message with info priority
	Ylgr::Warn(const char* message, Args... args)		// log a message with warn priority
	Ylgr::Error(const char* message, Args... args)		// log a message with error priority
	Ylgr::Critical(const char* message, Args... args)	// log a message with critical priority
```