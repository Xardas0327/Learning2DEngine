# DebugTool
- [DebugMacro](DebugTool.md#debugmacro)
- [Log](DebugTool.md#log)

##
## DebugMacro
### Source Code:
[DebugMacro.h](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugMacro.h)

### Description:
The L2DE_DEBUG is the only one, which defined everytime.
The other macros are defined only, when L2DE_DEBUG is 1.
Every macros are redefineable from the DebugMacro.

### Macros:
**L2DE_DEBUG**  
If _DEBUG is defined, the L2DE_DEBUG has same value.
If it is not defined, the L2DE_DEBUG is 0.  
But it can be overwritten if it is defined ealier. So it can be used in a Released Build too.

**L2DE_DEBUG_MAX_BASE_DELTATIME**  
In debug mode, the base delta time has a max value, because it can be huge if the game
stop by a breakpoint. Its default value is 0.1f.  
Note: the maximized delta time will be multiplied by timeScale.

##
## Log
### Source Code:
[Log.h](../../Learning2DEngine/Learning2DEngine/DebugTool/Log.h)

### Description:
The class contains static info, warning, and error log functions.
These functions are similar, just they use different colors and the text message is a bit different
in the console. But it is not recommended to use this function.
The `L2DE_LOG_INFO`, `L2DE_LOG_WARNING`, `L2DE_LOG_ERROR` macros are much better options for 2 reasons:
1. The developer does not have to care with the filePath and the fileLine parameters.
   The macros use the `__FILE__`, `__LINE__` by default.
2. If the `L2DE_DEBUG` is not defined
   as 1, the macros will become nothing, so the developer does not
   have to remove them for the release version.

### Header:
```cpp
class Log final
{...}
```

### Macros:
**L2DE_LOG_WARNING_COLOR**  
It change the color of the text to warning color.

**L2DE_LOG_ERROR_COLOR**  
It change the color of the text to error color.

**L2DE_LOG_CLEAR_COLOR**  
It change the color of the text to info (default) color.

**L2DE_LOG_INFO**  
It writes a Info message to terminal if the debug is activated.
```cpp
L2DE_LOG_INFO(message)
```

**L2DE_LOG_WARNING**  
It writes a Warning message to terminal if the debug is activated.
```cpp
L2DE_LOG_WARNING(message)
```

**L2DE_LOG_ERROR**  
It writes a Error message to terminal if the debug is activated.
```cpp
L2DE_LOG_ERROR(message)
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