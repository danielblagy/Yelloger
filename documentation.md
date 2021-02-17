# Yelloger

## Logging messages
```cpp
Log a message (format + optional args, follow printf specification) with log priority level Yellog::TracePriority
template<typename... Args>
static void Yellog::Trace(const char* message, Args... args)

// Log a message (format + optional args, follow printf specification)
// with log priority level Yellog::DebugPriority
template<typename... Args>
static void Debug(const char* message, Args... args)

// Log a message (format + optional args, follow printf specification)
// with log priority level Yellog::InfoPriority
template<typename... Args>
static void Info(const char* message, Args... args)

// Log a message (format + optional args, follow printf specification)
// with log priority level Yellog::WarnPriority
template<typename... Args>
static void Warn(const char* message, Args... args)

// Log a message (format + optional args, follow printf specification)
// with log priority level Yellog::ErrorPriority
template<typename... Args>
static void Error(const char* message, Args... args)

// Log a message (format + optional args, follow printf specification)
// with log priority level Yellog::CriticalPriority
template<typename... Args>
static void Critical(const char* message, Args... args)
```