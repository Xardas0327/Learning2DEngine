# System
- [BaseComponentHandler](System.md#basecomponenthandler)
- [BaseLateUpdaterComponent](System.md#baselateupdatercomponent)
- [BaseUpdaterComponent](System.md#baseupdatercomponent)
- [Camera](System.md#camera)
- [Component](System.md#component)
- [ComponentManager](System.md#componentmanager)
- [Game](System.md#game)
- [GameObject](System.md#gameobject)
- [IComponentHandler](System.md#icomponenthandler)
- [IKeyboardRefresher](System.md#ikeyboardrefresher)
- [InputStatus](System.md#inputstatus)
- [LateUpdaterComponent](System.md#lateupdatercomponent)
- [LateUpdaterComponentHandler](System.md#lateupdatercomponenthandler)
- [Random](System.md#random)
- [ResourceManager](System.md#resourcemanager)
- [Singleton](System.md#singleton)
- [Transform](System.md#transform)
- [UpdaterComponent](System.md#updatercomponent)
- [UpdaterComponentHandler](System.md#updatercomponenthandler)

##
## BaseComponentHandler
### Source Code:
[BaseComponentHandler.h](../../Learning2DEngine/Learning2DEngine/System/BaseComponentHandler.h)  

### Description:
It is a simple class for most component handlers.  
Please check for more info about the `ComponentManager`.

### Header:
```cpp
template<class T>
class BaseComponentHandler : public IComponentHandler
{...}
```

### Variables:
**Protected:**  
**components**  
```cpp
std::vector<T*> components;
```

**newComponents**  
```cpp
std::vector<T*> newComponents;
```

**removeableComponents**  
```cpp
std::vector<T*> removeableComponents;
```

### Functions:
**Protected:**  
**BaseComponentHandler**  
```cpp
BaseComponentHandler();
```

**RefreshComponents**  
It removes the `removeableComponents` and adds the `newComponents` to the `components`.
After this, it clears the `newComponents` and the `removeableComponents`.
```cpp
virtual void RefreshComponents();
```

**Public:**  
**Add**  
```cpp
virtual void Add(T* component);
```

**Remove**  
```cpp
virtual void Remove(T* component);
```

**Clear**  
```cpp
virtual void Clear() override;
```

##
## BaseLateUpdaterComponent
### Source Code:
[BaseLateUpdaterComponent.h](../../Learning2DEngine/Learning2DEngine/System/BaseLateUpdaterComponent.h)  

### Description:
It has some basic funcionality, which is essential, but this is a support class only,
please use `LateUpdaterComponent` instead of this.  
Please check for more info about `LateUpdaterComponent` and `Component`.

### Header:
```cpp
class BaseLateUpdaterComponent : public virtual Component
{...}
```

### Functions:
**Protected:**  
**BaseLateUpdaterComponent**  
```cpp
BaseLateUpdaterComponent(GameObject* gameObject);
```

**Public:**  
**LateUpdate**  
```cpp
virtual void LateUpdate() = 0;
```

##
## BaseUpdaterComponent
### Source Code:
[BaseUpdaterComponent.h](../../Learning2DEngine/Learning2DEngine/System/BaseUpdaterComponent.h)  

### Description:
It has some basic funcionality, which is essential, but this is a support class only,
please use `UpdaterComponent` instead of this.  
Please check for more info about `UpdaterComponent` and `Component`.

### Header:
```cpp
class BaseUpdaterComponent : public virtual Component
{...}
```

### Functions:
**Protected:**  
**BaseUpdaterComponent**  
```cpp
BaseUpdaterComponent(GameObject* gameObject);
```

**Public:**  
**Update**  
```cpp
virtual void Update() = 0;
```

##
## Camera
### Source Code:
[Camera.h](../../Learning2DEngine/Learning2DEngine/System/Camera.h)  
[Camera.cpp](../../Learning2DEngine/Learning2DEngine/System/Camera.cpp)

### Description:
The `Camera` contains the camera settings, which the developer can transform the view of the users.
The shaders can use its projection and its view matrix.
The developer can change the Camera's resolution, postion and rotation too.  
The `Game` class uses it as `mainCamera`.

### Header:
```cpp
class Camera
{...}
```

### Variables:
**Private:**  
**position**  
```cpp
glm::vec2 position;
```

**rotation**  
```cpp
float rotation;
```

**viewMatrix**  
```cpp
glm::mat4 viewMatrix;
```

**projection**  
```cpp
glm::mat4 projection;
```

**resolution**  
```cpp
Render::Resolution resolution;
```

### Functions:
**Private:**  
**RecalcViewMatrix**  
It recalculates the `viewMatrix`.
```cpp
void RecalcViewMatrix();
```

**Public:**  
**Camera**  
```cpp
Camera(glm::vec2 position = glm::vec2(0.0f, 0.0f), float rotation = 0.0f)
```

**GetPosition**  
It returns the `position`.
```cpp
inline glm::vec2 GetPosition();
```

**SetPosition**  
It sets the `position`.  
After the position update, it will call `RecalcViewMatrix`.
```cpp
void SetPosition(glm::vec2 position);
```

**Move**  
It adds the direction to the `position`.  
If the isWorldMoving is true, the direction will be rotated by the actual `rotation`.  
After the position update, it will call `RecalcViewMatrix`.
```cpp
void Move(glm::vec2 direction, bool isWorldMoving = false);
```

**GetRotation**  
It returns the `rotation`.
```cpp
inline float GetRotation();
```

**SetRotation**  
It sets the `rotation`.  
After the rotation update, it will call `RecalcViewMatrix`.
```cpp
void SetRotation(float rotation);
```

**Rotate**  
It add the angle to the `rotation`.  
After the rotation update, it will call `RecalcViewMatrix`.
```cpp
void Rotate(float angle);
```

**SetPositionRotation**  
It sets the `position` and the `rotation`.  
After the position and the rotation update, it will call `RecalcViewMatrix`.
```cpp
void SetPositionRotation(glm::vec2 position, float rotation);
```

**GetViewMatrix**  
It returns the `viewMatrix`.
```cpp
inline glm::mat4 GetViewMatrix();
```

**GetProjection**  
It returns the `projection`.
```cpp
inline glm::mat4 GetProjection();
```

**SetResolution**  
It sets the `resolution`.  
After the resolution update, it will recalculate the projection.
```cpp
void SetResolution(const Render::Resolution& resolution);
```

##
## Component
### Source Code:
[Component.h](../../Learning2DEngine/Learning2DEngine/System/Component.h)

### Description:
It is a base class for every component in the Engine.
Some base components are inherited from this class, which are used by `ComponentManager`.
That is why it is recommented to use these component classes, because they have functions,
which can run automatically:
- `System::UpdaterComponent`
- `System::LateUpdaterComponent`
- `Physics::BoxColliderComponent`
- `Physics::CircleColliderComponent`
- `Render::RendererComponent`
- `Render::LateRendererComponent`  

But the developer can use this `Component` class like a data container too.  
Fistly, it looks a bad structure, because there is a cross reference.
A `Component` has reference about its `GameObject` and the `GameObject` has 
reference about that `Component`. 
But if the developer follow some rules: 
1. The classes, which are inherited from `Component`, have to have a constructor and 
their first parameter is GameObject* for gameObject member.
2. It is recommended, that the constructor, the Init() and Destroy() of the inherited class is protected
and only the GameObject can use them.

If the developer follow these rules, only the `GameObject` can create/initialize/destroy `Components`,
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
**Public:**  
**isActive**  
It shows, that the actual component is active or not. If not, the `ComponentManager` will not
call the component's functions.
```cpp
bool isActive;
```  

**gameObject**  
```cpp
GameObject* const gameObject;
```

### Functions:
**Protected:**  
**Component**  
```cpp
Component(GameObject* gameObject);
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

##
## ComponentManager
### Source Code:
[ComponentManager.h](../../Learning2DEngine/Learning2DEngine/System/ComponentManager.h)  

### Description:
The `ComponentManager` manages the `Components` in the Engine by component handlers.
The `Game` calls its Update(), LateUpdate(), CheckCollision(), Render()
and LateRender() functions in every frame.

### Header:
```cpp
class ComponentManager final : public virtual Singleton<ComponentManager>
{...}
```

### Variables:
**Private:**  
**updaterComponentHandler**  
```cpp
UpdaterComponentHandler updaterComponentHandler;
```

**lateUpdaterComponentHandler**  
```cpp
LateUpdaterComponentHandler lateUpdaterComponentHandler;
```

**colliderComponentHandler**  
```cpp
Physics::ColliderComponentHandler colliderComponentHandler;
```

**rendererComponentHandler**  
```cpp
Render::RendererComponentHandler rendererComponentHandler;
```

**lateRendererComponentHandler**  
```cpp
Render::RendererComponentHandler lateRendererComponentHandler;
```

### Functions:
**Private:**  
**ComponentManager**  
```cpp
ComponentManager();
```

**Public:**  
**AddToUpdate**  
```cpp
inline void AddToUpdate(BaseUpdaterComponent* component);
```

**RemoveFromUpdate**  
```cpp
inline void RemoveFromUpdate(BaseUpdaterComponent* component);
```

**Update**  
```cpp
inline void Update();
```

**AddToLateUpdate**  
```cpp
inline void AddToLateUpdate(BaseLateUpdaterComponent* component);
```

**RemoveFromLateUpdate**  
```cpp
inline void RemoveFromLateUpdate(BaseLateUpdaterComponent* component);
```

**LateUpdate**  
```cpp
inline void LateUpdate();
```

**AddToCollider**  
```cpp
inline void AddToCollider(Physics::BaseBoxColliderComponent* component);
```
```cpp
inline void AddToCollider(Physics::BaseCircleColliderComponent* component);
```

**RemoveFromCollider**  
```cpp
inline void RemoveFromCollider(Physics::BaseBoxColliderComponent* component);
```
```cpp
inline void RemoveFromCollider(Physics::BaseCircleColliderComponent* component);
```

**CheckCollision**  
```cpp
inline void CheckCollision();
```

**AddToRenderer**  
```cpp
inline void AddToRenderer(Render::BaseRendererComponent* component);
```

**RemoveFromRenderer**  
```cpp
inline void RemoveFromRenderer(Render::BaseRendererComponent* component);
```

**NeedReorderRenderers**  
```cpp
inline void NeedReorderRenderers();
```

**Render**  
```cpp
inline void Render();
```

**AddToLateRenderer**  
```cpp
inline void AddToLateRenderer(Render::BaseRendererComponent* component);
```

**RemoveFromLateRenderer**  
```cpp
inline void RemoveFromLateRenderer(Render::BaseRendererComponent* component);
```

**NeedReorderLateRenderers**  
```cpp
inline void NeedReorderLateRenderers();
```

**LateRender**  
```cpp
inline void LateRender();
```

**Clear**  
Clear all handlers.
```cpp
void Clear();
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
2. Refresh Events (Keyboard, Mouse, Window etc)
3. Update
4. Check Collisions
5. LateUpdate
6. Clear Window to default color
7. Render (with MSAA and PostProcessEffect, if they are enabled)
8. LateRender (without any effect)
9. Update Window

### Header:
```cpp
class Game : public IKeyboardRefresher, public Render::IResolutionRefresher
{...}
```

### Macros:
**L2DE_KEYBOARD_BUTTON_NUMBER**  
The number of keyboard buttons.
Its value is 512.

**L2DE_TIME_SCALE_DEFAULT**  
The default value of the `timeScale`.
Its value is 1.0f.

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

**keyboardEventItem**  
It is used to subscribe to `RenderManager::AddKeyboardEvent`.
```cpp
EventSystem::KeyboardEventItem keyboardEventItem;
```

**resolutionEventItem** 
It is used to subscribe to `RenderManager::AddFramebufferSizeEvent`. 
```cpp
EventSystem::ResolutionEventItem resolutionEventItem;
```

**keyboardButtons**  
This array contains, which keyboard button is up, down or hold.
```cpp
static InputStatus keyboardButtons[L2DE_KEYBOARD_BUTTON_NUMBER];
```

**deltaTime**  
It is multiplied by `timeScale`.  
Before the first frame, it is 0.0f.
```cpp
static float deltaTime;
```

**Public:**  
**mainCamera**  
The `mainCamera` contains the camera settings,
which the developer can transform the view of the users.  
The shaders can use its projection and its view matrix.  
Its resolution has to be initialized. It is (0,0) by default.  
Please check for more info about the `Camera` class.
```cpp
static Camera mainCamera;
```

### Functions:
**Private:**  
**UpdateEvents**  
```cpp
void UpdateEvents();
```

**FixKeyboardButtons**  
The *glfwPollEvents* does not refresh the data on every frame.
That's why this function update the `InputStatus::KEY_DOWN`
to `InputStatus::KEY_HOLD`.
```cpp
void FixKeyboardButtons();
```

**Protected:**  
**Game**  
```cpp
 Game();
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

**Public:**  
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

**RefreshKeyboard**  
The developer should not use this function.
The `Game` subscribes for button events and the `RenderManager` call
this function by an event.
```cpp
void RefreshKeyboard(int key, int scancode, int action, int mode) override;
``` 

**RefreshResolution**  
The developer should not use this function.
The `Game` subscribe for (game) screen resolution events
and the `RenderManager` call this function by an event.
```cpp
virtual void RefreshResolution(const Render::Resolution& resolution) override;
``` 

**GetDeltaTime**  
It returns the deltaTime.  
Please check for more info about deltaTime.
```cpp
static float GetDeltaTime();
``` 

**GetKeyboardButtonStatus**  
It returns the status of a keyboard button.  
```cpp
static InputStatus GetKeyboardButtonStatus(int key);
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
It shows, that the actual game object is active or not.
If not, the `ComponentManager` will not call the components of the game object.
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
## IComponentHandler
### Source Code:
[IComponentHandler.h](../../Learning2DEngine/Learning2DEngine/System/IComponentHandler.h)

### Description:
It is a little interface, which contains the two main functions of the component handlers.

### Header:
```cpp
class IComponentHandler
{...}
```

### Functions:
**Public:**  
**~IComponentHandler**  
```cpp
virtual ~IComponentHandler();
```

**Clear**  
```cpp
virtual void Clear() = 0;
```

**DoWithAllComponents**  
```cpp
virtual void DoWithAllComponents() = 0;
```


##
## IKeyboardRefresher
### Source Code:
[IKeyboardRefresher.h](../../Learning2DEngine/Learning2DEngine/System/IKeyboardRefresher.h)

### Description:
It is a little interface, which the developer can use to wrap
a class into `KeyboardEventItem`.

### Header:
```cpp
class IKeyboardRefresher
{...}
```

### Functions:
**Public:**  
**~IKeyboardRefresher**  
```cpp
virtual ~IKeyboardRefresher();
```

**RefreshKeyboard**  
```cpp
virtual void RefreshKeyboard(int key, int scancode, int action, int mode) = 0;
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
    // The key was held down.
    KEY_HOLD
};
```

##
## LateUpdaterComponent
### Source Code:
[LateUpdaterComponent.h](../../Learning2DEngine/Learning2DEngine/System/LateUpdaterComponent.h)

### Description:
It is a class, which is inherited from `BaseLateUpdaterComponent`.  
The developer have to inherit from this class, if they want to do something
in LateUpdate section, after the collision checking.    
Please check for more info about `System::Component` and `BaseLateUpdaterComponent`.

### Functions:
**Protected:**  
**LateUpdaterComponent**  
```cpp
LateUpdaterComponent(GameObject* gameObject);
```

**Init**  
If this function is override, it must call the LateUpdaterComponent::Init()
in the first line.
```cpp
virtual void Init() override;
```

**Destroy**  
If this function is override, it must call the LateUpdaterComponent::Destroy()
in the first line.
```cpp
virtual void Destroy() override;
```

##
## LateUpdaterComponentHandler
### Source Code:
[LateUpdaterComponentHandler.h](../../Learning2DEngine/Learning2DEngine/System/LateUpdaterComponentHandler.h)  

### Description:
It can handle the `BaseLateUpdaterComponent` objects.  
The `ComponentManager` has one from it.

### Header:
```cpp
class LateUpdaterComponentHandler : public virtual BaseComponentHandler<BaseLateUpdaterComponent>
{...}
```

### Functions:
**Public:**  
**LateUpdaterComponentHandler**  
```cpp
LateUpdaterComponentHandler();
```

**DoWithAllComponents**  
Firstly it refresh the `BaseLateUpdaterComponent` objects.  
After that it calls the LateUpdate() function of the objects
if the object is not in the removeableComponents and the component and its gameObject are active.  
Note: the code have to check the removeableComponents again, because
maybe another component removed/destroyed the actual component in actual frame.
```cpp
void DoWithAllComponents() override;
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

##
## UpdaterComponent
### Source Code:
[UpdaterComponent.h](../../Learning2DEngine/Learning2DEngine/System/UpdaterComponent.h)

### Description:
It is a class, which is inherited from `BaseUpdaterComponent`.  
The developer have to inherit from this class, if they want to do something
in Update section, before the collision checking.    
Please check for more info about `System::Component` and `BaseUpdaterComponent`.

### Functions:
**Protected:**  
**UpdaterComponent**  
```cpp
UpdaterComponent(GameObject* gameObject);
```

**Init**  
If this function is override, it must call the UpdaterComponent::Init()
in the first line.
```cpp
virtual void Init() override;
```

**Destroy**  
If this function is override, it must call the UpdaterComponent::Destroy()
in the first line.
```cpp
virtual void Destroy() override;
```

##
## UpdaterComponentHandler
### Source Code:
[UpdaterComponentHandler.h](../../Learning2DEngine/Learning2DEngine/System/UpdaterComponentHandler.h)  

### Description:
It can handle the `BaseUpdaterComponentHandler` objects.  
The `ComponentManager` has one from it.

### Header:
```cpp
class UpdaterComponentHandler : public virtual BaseComponentHandler<BaseUpdaterComponent>
{...}
```

### Functions:
**Public:**  
**UpdaterComponentHandler**  
```cpp
UpdaterComponentHandler();
```

**DoWithAllComponents**  
Firstly it refresh the `BaseUpdaterComponent` objects.  
After that it calls the Update() function of the objects
if the object is not in the removeableComponents and the component and its gameObject are active.  
Note: the code have to check the removeableComponents again, because
maybe another component removed/destroyed the actual component in actual frame.
```cpp
void DoWithAllComponents() override;
```
