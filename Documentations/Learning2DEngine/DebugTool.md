# DebugTool

## Log
### Source Code:
[Log.h](../../Learning2DEngine/Learning2DEngine/DebugTool/Log.h)

### Description:
The class contains static info, warning, and error log functions.
These functions are similar, just they use different colors and the text message is a bit different
in the console. But it is not recommended to use this function.
The `LOG_INFO`, `LOG_WARNING`, `LOG_ERROR` macros are much better options for 2 reasons:
1. The developer does not have to care with the filePath and the fileLine parameters.
   The macros use the `__FILE__`, `__LINE__` by default.
2. The `_DEBUG` (Visual Studio uses it in debug build) or the `LEARNING2DENGINE_DEBUG` is not defined
   as true, the macros will become nothing, so the developer does not
   have to remove them for the release version.

### Header:
```cpp
class Log final
{...}
```

### Macros:
**LOG_WARNING_COLOR**  
It change the color of the text to warning color.

**LOG_ERROR_COLOR**  
It change the color of the text to error color.

**LOG_CLEAR_COLOR**  
It change the color of the text to info (default) color.

**LOG_INFO**  
It writes a Info message to terminal if the debug is activated.
```cpp
LOG_INFO(message)
```

**LOG_WARNING**  
It writes a Warning message to terminal if the debug is activated.
```cpp
LOG_WARNING(message)
```

**LOG_ERROR**  
It writes a Error message to terminal if the debug is activated.
```cpp
LOG_ERROR(message)
```

### Functions:
**Private:**  
**Log**  
```cpp
Log();
```

**GetMessage**  
```cpp
static std::string GetMessage(const std::string& message, const std::string& filePath, int fileLine);
```

**Public:**  

**Info**  
It writes a Info message to terminal.
```cpp
static void Info(const std::string& message, const std::string& filePath, int fileLine);
```

**Warning**  
It writes a Warning message to terminal.
```cpp
static void Warning(const std::string& message, const std::string& filePath, int fileLine);
```

**Error**  
It writes a Error message to terminal.
```cpp
static void Error(const std::string& message, const std::string& filePath, int fileLine);
```