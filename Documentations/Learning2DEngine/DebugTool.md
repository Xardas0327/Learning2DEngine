# DebugTool
- [DebugBoxColliderRenderComponent](DebugTool.md#debugboxcolliderrendercomponent)
- [DebugBoxColliderRenderer](DebugTool.md#debugboxcolliderrenderer)
- [DebugCircleColliderRenderComponent](DebugTool.md#debugcirclecolliderrendercomponent)
- [DebugCircleColliderRenderer](DebugTool.md#debugcirclecolliderrenderer)
- [DebugColliderRenderer](DebugTool.md#debugcolliderrenderer)
- [DebugMacro](DebugTool.md#debugmacro)
- [DebugRenderData](DebugTool.md#debugrenderdata)
- [Log](DebugTool.md#log)


##
## DebugBoxColliderRenderComponent
### Source Code:
[DebugBoxColliderRenderComponent.h](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugBoxColliderRenderComponent.h)  
[DebugBoxColliderRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugBoxColliderRenderComponent.cpp)

### Description:
A component, which is used to render the box colliders.

### Header:
```cpp
class DebugBoxColliderRenderComponent : public Render::RendererComponent<DebugRenderData<Physics::BaseBoxColliderComponent>, DebugBoxColliderRenderer>
{...}
```

### Variables:
**Private:**  
**id**  
```cpp
static const std::string id;
```

### Functions:
**Protected:**  
**DebugBoxColliderRenderComponent**  
```cpp
DebugBoxColliderRenderComponent(System::GameObject* gameObject, Physics::BaseBoxColliderComponent* collider);
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetInitedRenderer**  
```cpp
DebugBoxColliderRenderer* GetInitedRenderer() override;
```

**DestroyRenderer**  
```cpp
void DestroyRenderer() override;
```

##
## DebugBoxColliderRenderer
### Source Code:
[DebugBoxColliderRenderer.h](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugBoxColliderRenderer.h)  
[DebugBoxColliderRenderer.cpp](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugBoxColliderRenderer.cpp)

### Description:
This renderer is used by `DebugBoxColliderRenderComponent`, that it can render the box colliders.

### Header:
```cpp
class DebugBoxColliderRenderer : public DebugColliderRenderer<Physics::BaseBoxColliderComponent>, public System::Singleton<DebugBoxColliderRenderer>
{...}
```

### Functions:
**Private:**  
**DebugBoxColliderRenderer**  
```cpp
DebugBoxColliderRenderer();
```

**InitVao**  
```cpp
void InitVao();
```

**Public:**  
**Draw**  
It draws those objects, which was added with SetData and they are on the selected layer.
```cpp
void Draw(int layer) override;
```

##
## DebugCircleColliderRenderComponent
### Source Code:
[DebugCircleColliderRenderComponent.h](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugCircleColliderRenderComponent.h)  
[DebugCircleColliderRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugCircleColliderRenderComponent.cpp)

### Description:
A component, which is used to render the circle colliders.

### Header:
```cpp
class DebugCircleColliderRenderComponent : public Render::RendererComponent<DebugRenderData<Physics::BaseCircleColliderComponent>, DebugCircleColliderRenderer>
{...}
```

### Variables:
**Private:**  
**id**  
```cpp
static const std::string id;
```

### Functions:
**Protected:**  
**DebugCircleColliderRenderComponent**  
```cpp
DebugCircleColliderRenderComponent(System::GameObject* gameObject, Physics::BaseCircleColliderComponent* collider);
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetInitedRenderer**  
```cpp
DebugCircleColliderRenderer* GetInitedRenderer() override;
```

**DestroyRenderer**  
```cpp
void DestroyRenderer() override;
```

##
## DebugCircleColliderRenderer
### Source Code:
[DebugCircleColliderRenderer.h](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugCircleColliderRenderer.h)  
[DebugCircleColliderRenderer.cpp](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugCircleColliderRenderer.cpp)

### Description:
This renderer is used by `DebugCircleColliderRenderComponent`, that it can render the circle colliders.

### Header:
```cpp
class DebugCircleColliderRenderer : public DebugColliderRenderer<Physics::BaseCircleColliderComponent>, public System::Singleton<DebugCircleColliderRenderer>
{...}
```

### Macros:
**L2DE_DEBUG_CIRCLE_SEGMENT**  
It is a segment of the circle.  
If it is bigger, a circle will be smoother.  
It's value is 50.

### Functions:
**Private:**  
**DebugCircleColliderRenderer**  
```cpp
DebugCircleColliderRenderer();
```

**InitVao**  
```cpp
void InitVao();
```

**Public:**  
**Draw**  
It draws those objects, which was added with SetData and they are on the selected layer.
```cpp
void Draw(int layer) override;
```

##
## DebugColliderRenderer
### Source Code:
[DebugColliderRenderer.h](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugColliderRenderer.h)  

### Description:
The parent class for debug collider renderers.

### Header:
```cpp
template<class TColliderComponent>
class DebugColliderRenderer : public Render::BaseMultiRenderer<Render::BaseColorDynamicData>
{...}
```

### Variables:
**Protected:**  
**debugRenderData**  
When the SetData is called, the renderData will be converted to this format.  
The int is the layer.
```cpp
std::map<int, std::vector<DebugRenderData<TColliderComponent>*>> debugRenderData;
```

### Functions:
**Protected:**  
**DebugColliderRenderer**  
```cpp
DebugColliderRenderer();
```

**InitShader**  
```cpp
void InitShader() override;
```

**DestroyObject**  
The Destroy() call it with or without mutex.
```cpp
void DestroyObject() override;
```

**Public:**
**SetData**  
This function converts the renderData to debugRenderData.
```cpp
void SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData) override;
```

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

**L2DE_DEBUG_SHOW_COLLIDER**  
If it is defined, the colliders can be visible in the game.

**L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_VALUE**  
If it is 1 (or true), the colliders will be visible in the game by default.  
If is is 0 (or false), the colliders will be invisible in the game by default.  
But the developer can change them one by one.

**L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_LAYER**  
The colliders are rendered by a renderer (not late renderer), and this macro give their default layer.

**L2DE_DEBUG_SHOW_COLLIDER_COLOR**  
The default color of the colliders, which use collider mode.

**L2DE_DEBUG_SHOW_COLLIDER_TRIGGER_COLOR**  
The default color of the triggers. (Colliders with trigger mode)

##
## DebugRenderData
### Source Code:
[DebugRenderData.h](../../Learning2DEngine/Learning2DEngine/DebugTool/DebugRenderData.h)

### Description:
The debug collider renderers use it.  
The TComponent has to be a component, which is derived from System::Component.

### Header:
```cpp
template<class TComponent>
struct DebugRenderData : public Render::RenderData
{
	const TComponent* const objectComponent;

	DebugRenderData(const System::Component* component, const TComponent* objectComponent)
		: RenderData(component), objectComponent(objectComponent)
	{
	}
};
```

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