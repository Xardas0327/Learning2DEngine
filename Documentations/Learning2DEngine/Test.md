# Test
It contains any helper classes for the testing.

- [CompareFloat](Test.md#comparefloat)

##
## CompareFloat
### Source Code:
[CompareFloat.h](../../Learning2DEngineTest/Test/CompareFloat.h)  
[CompareFloat.cpp](../../Learning2DEngineTest/Test/CompareFloat.cpp)

### Description:
When float variables is modified, maybe it won't be
exactly that number, which it should be.  
For example: 1.2f => 1.1999999f.

### Header:
```cpp
class CompareFloat
{...}
```

### Variables:
**Private:**  
**floatThreshold**  
The difference, which is still accepted.  
It is 0.001f.
```cpp
static const float floatThreshold;
```

### Functions:
**Public:**  
**IsEqual**  
```cpp
static bool IsEqual(float a, float b);
```
```cpp
static bool IsEqual(const glm::vec2& a, const glm::vec2& b);
```
```cpp
static bool IsEqual(const glm::vec4& a, const glm::vec4& b);
```
```cpp
static bool IsEqual(const glm::mat4& a, const glm::mat4& b);
```
```cpp
static bool IsEqual(const glm::mat4x2& a, const glm::mat4x2& b);
```