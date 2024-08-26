# System
- [Component](Render.md#component)
- [Game](Render.md#game)

##
## Component
### Source Code:
[Component.h](../../Learning2DEngine/Learning2DEngine/System/Component.h)

### Description:
Fistly, it looks a bad structure, because there is a cross reference.
A `Component` has reference about its `GameObject` and the `GameObject` has 
reference about that `Component`.  
But if the developer follow some rules, everything will be alright.
The classes, which are inherited from `Component`, have to have a constructorand 
their first parameter is GameObject* for gameObject member.
Moreover, It is recommended, that the constructor, the Init() and Destroy() of the inherited class is protected
and only the GameObject can use them.  
If the developer follow these, only the `GameObject` can create/initialize/destroy `Components`,
so a `Component` can not exist without its `GameObject`.  
Furthermore, the `Components` have reference about their gameobject, so this gave the developer a huge flexibility.
They can give or get components from the their `GameObject` within a `Component`.  
Please check the `GameObject` too.

### Header:
```cpp
class Component
{...}
```

### Variables:
**Protected:**  
**isActive**  
```cpp
bool isActive;
```

**Public:**  
**gameObject**  
```cpp
GameObject* const gameObject;
```

### Functions:
**Protected:**  
**Component**  
```cpp
Component(GameObject* gameObject)
```

**Init**  
It initializes the Component.
```cpp
virtual void Init();
```

**Destroy**  
It destroys the Component.
```cpp
virtual void Destroy();
```

**Public:**  
**~Component**  
```cpp
virtual ~Component();
```

**SetActive**  
It changes the isActive.
```cpp
inline void SetActive(bool value);
```

**GetActive**  
It returns true, if the isActive is true.
```cpp
inline bool GetActive();
```

##
## Game
### Source Code:
[Game.h](../../Learning2DEngine/Learning2DEngine/System/Game.h)  
[Game.cpp](../../Learning2DEngine/Learning2DEngine/System/Game.cpp)  

### Description:
The `Game` class is, which holds everything together in the Engine.
If a developer want to create a game by this Engine,
that game must be inherited from this `Game` class.   

The Function order in the Run() (in a frame):
1. Calculate deltaTime
2. Refresh Keyboard and Mouse events
3. virtual Update()
4. Clear Window to default color
5. virtual Render() (Render with MSAA and PostProcessEffect, if they are enabled)
6. virtual LateRender (Render without any effect)
7. Update Window

### Header:
```cpp
class Game : public virtual IKeyboardMouseRefresher, public virtual Render::IResolutionRefresher
{...}
```

### Macros:
**INPUT_KEY_SIZE**  
The number of keyboard and mouse buttons.

**TIME_SCALE_DEFAULT**  
The default value of the `timeScale`.

### Variables:
**Private:**  
**lastFrame**  
```cpp
 float lastFrame;
```

**timeScale**  
```cpp
float timeScale;
```

**isMsaaActive**  
```cpp
bool isMsaaActive;
```

**isPostProcessEffectActive**  
It contains, that the `ppeRender` is initialized or not.
```cpp
bool isPostProcessEffectActive;
```

**isPostProcessEffectUsed**  
It contains, that the `ppeRender` is used or not.
For using, it has to be initialized.
```cpp
bool isPostProcessEffectUsed;
```

**msaaRender**  
```cpp
Render::MSAA msaaRender;
```

**ppeRender**  
```cpp
Render::PostProcessEffect ppeRender;
```

**keyboardMouseEventItem**  
It is used to subscribe to `RenderManager::AddKeyboardEvent`.
```cpp
EventSystem::KeyboardMouseEventItem keyboardMouseEventItem;
```

**resolutionEventItem** 
It is used to subscribe to `RenderManager::AddFramebufferSizeEvent`. 
```cpp
EventSystem::ResolutionEventItem resolutionEventItem;
```

**deltaTime**  
It is multiplied by `timeScale`.  
Before the first frame, it is 0.0f.
```cpp
static float deltaTime;
```

**Protected:** 
**inputKeys**  
This array contains, which button is up, down or hold.  
The developer should not write this array, just read it.
```cpp
InputStatus inputKeys[INPUT_KEY_SIZE];
```

**cameraProjection**  
The camera projecttion, which used by the shaders.
The developer can modify, what the player can see.  
It has to be initialized. It is glm::mat4(0.0f) by default.
```cpp
static glm::mat4 cameraProjection;
```

**cameraResolution**  
It has to be initialized,
because the camera resolution don't have to be same with game resolution.
It is (0,0) by default. The developer can use it as a constant grid size
if they do not change it.  
For example: the default (game) screen and camera resolution is 800x600 and
there is a text in the middle of the Game Window.
If the player change the resolution of the game, the camera resolution
won't change, so the text will stay in the middle of the Game Window.
```cpp
static Render::Resolution cameraResolution;
```

### Functions:
**Private:**  
**UpdateKeyboardMouseEvents**  
```cpp
void UpdateKeyboardMouseEvents();
```

**FixKeyboardMouse**  
The *glfwPollEvents* does not refresh the data on every frame.
That's why this function update the `InputStatus::KEY_DOWN`
to `InputStatus::KEY_HOLD`.
```cpp
void FixKeyboardMouse();
```

**Protected:**  
**Update**  
It is recommended, that this function should contain every update in the game,
which is not rendering.
```cpp
virtual void Update();
``` 

**Render**  
It is recommended, that this function should contain every rendering functions,
which render the game scene. It uses the MSAA and PostProcessEffect,
if they are enabled.  
But there are some exception, please check the `LateRender()`.
```cpp
virtual void Render();
``` 
**LateRender**  
It is also a rendering function.
But this does not use the the MSAA and PostProcessEffect.  
This can be useful for UI.
```cpp
virtual void LateRender();
``` 

**ActivateMSAA**  
It activates the MSAA.
```cpp
 void ActivateMSAA(unsigned int sampleNumber);
``` 

**StopMSAA**  
It stops the MSAA.
```cpp
void StopMSAA();
``` 

**IsMsaaActive**  
It returns true, if the MSAA is activated.
```cpp
inline bool IsMsaaActive();
``` 

**ActivatePostProcessEffect**  
It initializes the PostProcessEffect.  
By default, it renders the game screen without any modification.
```cpp
void ActivatePostProcessEffect();
``` 

**StopPostProcessEffect**  
It destroys the PostProcessEffect.
```cpp
void StopPostProcessEffect();
``` 

**IsPostProcessEffectActive**  
It returns true, if the PostProcessEffect is initialized.
```cpp
inline bool IsPostProcessEffectActive();
``` 

**UsePostProcessEffect**  
It start to use the new effect shader for post processing.
```cpp
inline void UsePostProcessEffect();
``` 
```cpp
void UsePostProcessEffect(const Render::Shader& shader);
``` 

**NotUsePostProcessEffect**  
It start to use the default shader.
(It renders the game screen without any modification).  
But the effect shader still exist.
```cpp
inline void NotUsePostProcessEffect();
``` 

**ClearPostProcessEffect**  
It start to use the default shader and clear the effect shader.
(It renders the game screen without any modification).  
```cpp
void ClearPostProcessEffect();
``` 

**IsPostProcessEffectUsed**  
It returns true, if the effect shader is used. 
```cpp
inline bool IsPostProcessEffectUsed();
``` 

**ResetTimeScale**  
It resets the `timeScale` by `TIME_SCALE_DEFAULT`.
```cpp
inline void ResetTimeScale();
``` 

**SetTimeScale**  
It sets the `timeScale`.
```cpp
inline void SetTimeScale(float value);
``` 

**GetTimeScale**  
It returns the `timeScale`.
```cpp
inline float GetTimeScale();
``` 

**SetCameraResolution**  
It sets the `cameraResolution` and the `cameraProjection` too.
```cpp
void SetCameraResolution(const Render::Resolution& resolution);
``` 

**Public:** 
**Game**  
```cpp
 Game();
```  

**~Game**  
```cpp
virtual ~Game();
```  

**InitWithRender**  
It initializes the `RenderManager`. After that, the `Init()` will be called.
```cpp
void InitWithRender(int majorRenderVersion, int minorRenderVersion, Render::Resolution resolution, const char* title, bool resizableWindows = true);
``` 

**Init**  
It initializes the `Game`, but the `RenderManager` have to be initialized
before this. If this function is override, it must call the `Game::Init()`
in the first line.  
The Blend is enable
by default with Source: *SRC_ALPHA*, Destination: *ONE_MINUS_SRC_ALPHA*,
because it is used for some features.
```cpp
virtual void Init();
``` 

**Terminate**  
It terminates the `Game`.  
If this function is override, it must call the `Game::Terminate()`
in the last line.
```cpp
virtual void Terminate();
``` 

**Run**  
It runs the `Game`.
It has an infinite loop, that's why it will run until the game window is closed.
Please check the Game's Description for the function order.
```cpp
void Run();
``` 

**RefreshKeyboardMouse**  
The developer should not use this function.
The `Game` subscribes for button events and the `RenderManager` call
this function by an event.
```cpp
void RefreshKeyboardMouse(int key, int scancode, int action, int mode) override;
``` 

**RefreshResolution**  
The developer should not use this function.
The `Game` subscribe for (game) screen resolution events
and the `RenderManager` call this function by an event.
```cpp
virtual void RefreshResolution(const Render::Resolution& resolution) override;
``` 

**GetDeltaTime**  
It returns the `deltaTime`.  
Please check more info about `deltaTime`.
```cpp
static float GetDeltaTime();
``` 

**GetCameraProjection**  
It returns the `cameraProjection`.
```cpp
static glm::mat4 GetCameraProjection();
``` 

**GetCameraResolution**  
It returns the `cameraResolution`.
```cpp
static Render::Resolution GetCameraResolution();
``` 