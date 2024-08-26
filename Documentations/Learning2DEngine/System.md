# System
- [Component](System.md#component)
- [Game](System.md#game)
- [GameObject](System.md#gameobject)
- [IKeyboardMouseRefresher](System.md#ikeyboardmouserefresher)
- [InputStatus](System.md#inputstatus)
- [Random](System.md#random)
- [ResourceManager](System.md#resourcemanager)
- [Singleton](System.md#singleton)
- [Transform](System.md#transform)

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

##
## GameObject
### Source Code:
[GameObject.h](../../Learning2DEngine/Learning2DEngine/System/GameObject.h)

### Description:
The `GameObject` represent the objects in the game.
It contains the components, which give a big flexibility to the developer.  
Please check the `Component` too.

### Header:
```cpp
class GameObject final
{...}
```

### Variables:
**Private:**  
**components**  
```cpp
std::vector<Component*> components;
```

**Public:**  
**isActive**  
```cpp
bool isActive;
```

**transform**  
```cpp
Transform transform;
```

### Functions:
**Public:**  
**GameObject**  
```cpp
GameObject(bool isActive = true);
```
```cpp
GameObject(Transform transform, bool isActive = true);
```

**~GameObject()**  
It calls `Destroy()` function of the current gameobject's components
and delete them.
```cpp
~GameObject();
```

**AddComponent**  
It adds a `Component` to the `GameObject` and
it calls the `Init()` function of the `Component`.
This is a really flexible function.
The developer can add parameters to component's constructor.  
For example:
A Component constructor:
PlayerController(GameObject* gameObject, const std::string& textureId);  
The textureId can be added by this format:
```cpp
GameObject* player = new GameObject();
playerController = player->AddComponent<PlayerController, const std::string&>("paddle");
```
```cpp
template <class TComponent, class ...Params>
TComponent* AddComponent(Params... params);
```

**GetComponent**  
They return the `Component`, whose class is *TComponent*.
If the gameobject does not have this `Component`, it will return *NULL*.
```cpp
template <class TComponent>
TComponent* GetComponent();
```
```cpp
template <class TComponent>
const TComponent* GetComponent() const;
```

**GetComponents**  
It returns all components, whose class is *TComponent*.
If the `GameObject` does not have these components,
it will return an empty vector.
```cpp
template <class TComponent>
std::vector<TComponent*> GetComponents();
```

**Destroy**  
It destroys the `GameObject`.  
If the developer give a `Component`, it will destroy
the `GameObject` of the `Component`.  
That's why all other `Components`,which the `GameObject` has, will be destroyed.
```cpp
static void Destroy(GameObject* gameObject);
```
```cpp
static void Destroy(Component* component);
```

##
## IKeyboardMouseRefresher
### Source Code:
[IKeyboardMouseRefresher.h](../../Learning2DEngine/Learning2DEngine/System/IKeyboardMouseRefresher.h)

### Description:
It is a little interface, which the developer can use to wrap
a class into `KeyboardMouseEventItem`.

### Header:
```cpp
class IKeyboardMouseRefresher
{...}
```

### Functions:
**Public:**  
**~IKeyboardMouseRefresher**  
```cpp
virtual ~IKeyboardMouseRefresher();
```

**RefreshKeyboardMouse**  
```cpp
virtual void RefreshKeyboardMouse(int key, int scancode, int action, int mode) = 0;
```

##
## InputStatus
### Source Code:
[InputStatus.h](../../Learning2DEngine/Learning2DEngine/System/InputStatus.h)

### Description:
It is an enum class, which contains the 3 phase of the buttons.

### Header:
```cpp
enum InputStatus
{
    // The key or mouse button was released.
    KEY_UP,
    // The key or mouse button was pressed.
    KEY_DOWN,
    // The key was held down until it repeated.
    KEY_HOLD
};
```

##
## Random
### Source Code:
[Random.h](../../Learning2DEngine/Learning2DEngine/System/Random.h)  
[Random.cpp](../../Learning2DEngine/Learning2DEngine/System/Random.cpp)

### Description:
It is a static random number generator.
It uses the *rand()* function from *cstdlib*.
So, the randomness is depend on *RAND_MAX*. If the isInited is false,
the next `GetNumber` function will run the *std::srand(std::time(nullptr))*,
before generation.

### Header:
```cpp
class Random
{...}
```

### Variables:
**Public:**  
**isInited**  
```cpp
static bool isInited;
```

### Functions:
**Private:**  
**Random**  
```cpp
Random();
```

**Init**  
```cpp
static void Init();
```

**Public:**  
**GetNumber**  
```cpp
static int GetNumber(int minInclusive, int maxExclusive);
```
```cpp
static float GetNumber(float minInclusive, float maxInclusive);
```

##
## ResourceManager
### Source Code:
[ResourceManager.h](../../Learning2DEngine/Learning2DEngine/System/ResourceManager.h)  
[ResourceManager.cpp](../../Learning2DEngine/Learning2DEngine/System/ResourceManager.cpp)  

### Description:
It is really recommended, that the developer create shaders
and textures with the `ResourceManager`, that
they can avoid the useless allocation and memory leak.
The exception should be minimum.  
It use one-one *std::map* for shaders and textures, where the id is their name.

### Header:
```cpp
class ResourceManager : public virtual Singleton<ResourceManager>
{...}
```

### Variables:
**Private:**  
**shaders**  
```cpp
std::map<std::string, Render::Shader> shaders;
```

**textures**  
```cpp
std::map<std::string, Render::Texture2D> textures;
```

### Functions:
**Private:**  
**ResourceManager**  
```cpp
ResourceManager();
```

**LoadShaderFile**  
Return false, if the *std\::ifstream* or the *std\::stringstream* thrown exception
by *std\::ifstream::failbit* or *std\::ifstream::badbit*.  
Otherwise, it returns true.
```cpp
bool LoadShaderFile(const char* filePath, std::string& outSource);
```

**LoadShaderFromFile**  
```cpp
Render::Shader LoadShaderFromFile(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath = nullptr);
```

**LoadShader**  
```cpp
Render::Shader LoadShader(const char* vertexText, const char* fragmentText, const char* geometryText = nullptr);
```

**LoadTextureFromFile**  
```cpp
Render::Texture2D LoadTextureFromFile(const char* filePath, const Render::Texture2DSettings& settings);
```

**Public:**  
**LoadShaderFromFile**  
It creates a `Shader` from files.
```cpp
 Render::Shader LoadShaderFromFile(const std::string& name, const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath = nullptr);
```

**LoadShader**  
It creates a `Shader` from strings.
```cpp
Render::Shader LoadShader(const std::string& name, const char* vertexText, const char* fragmentText, const char* geometryText = nullptr);
```

**GetShader**  
It returns a `Shader`. If the shader does not exist,
it will return an uninitialized `Shader`.  
It is recommended to use the `IsShaderExist()`.
```cpp
Render::Shader GetShader(const std::string& name);
```

**IsShaderExist**  
It returns true if the `Shader` exist.
```cpp
bool IsShaderExist(const std::string& name);
```

**DestroyShader**  
It destroys the `Shader`.
```cpp
void DestroyShader(const std::string& name);
```

**LoadTextureFromFile**  
It creates a `Texture2D` from files.
```cpp
Render::Texture2D LoadTextureFromFile(const std::string& name, const char* filePath, const Render::Texture2DSettings& settings);
```

**GetTexture**  
It returns a `Texture2D`. If the texture does not exist,
it will return an uninitialized `Texture2D`.
It is recommended to use the `IsTextureExist()`.
```cpp
Render::Texture2D GetTexture(const std::string& name);
```

**IsTextureExist**  
It returns true if the `Texture2D` exist.
```cpp
bool IsTextureExist(const std::string& name);
```

**DestroyTexture2D**  
It destroys the `Texture2D`.
```cpp
void DestroyTexture2D(const std::string& name);
```

**Clear**  
It destroys all `Shader` and `Texture2D`.
```cpp
void Clear();
```

##
## Singleton
### Source Code:
[Singleton.h](../../Learning2DEngine/Learning2DEngine/System/Singleton.h)  

### Description:
A base singleton abstract class. It is recommended, that
the constructor of inherited class is private or protected and
the `Singleton` is a friend class.

### Header:
```cpp
template<typename T>
class Singleton
{...}
```

### Functions:
**Protected:**  
**Singleton**  
```cpp
Singleton();
```

**Public:**  
**GetInstance**  
```cpp
static T& GetInstance();
```

##
## Transform
### Source Code:
[Transform.h](../../Learning2DEngine/Learning2DEngine/System/Transform.h)  

### Description: 
It contains the position the scale and the rotation.  
Every `GameObject` has a `Transform`.

### Header:
```cpp
struct Transform
{
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;

	Transform(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f)
		: position(position), scale(scale), rotation(rotation)
	{

	}
};
```