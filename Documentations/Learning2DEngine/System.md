# System
- [BaseComponentHandler](System.md#basecomponenthandler)
- [BaseLateUpdaterComponent](System.md#baselateupdatercomponent)
- [BaseUpdaterComponent](System.md#baseupdatercomponent)
- [Camera](System.md#camera)
- [Component](System.md#component)
- [ComponentManager](System.md#componentmanager)
- [Cursor](System.md#cursor)
- [Game](System.md#game)
- [GameObject](System.md#gameobject)
- [GameObjectManager](System.md#gameobjectmanager)
- [IComponentHandler](System.md#icomponenthandler)
- [ICursorRefresher](System.md#icursorrefresher)
- [IKeyboardRefresher](System.md#ikeyboardrefresher)
- [InputStatus](System.md#inputstatus)
- [LateUpdaterComponent](System.md#lateupdatercomponent)
- [LateUpdaterComponentHandler](System.md#lateupdatercomponenthandler)
- [Math](System.md#math)
- [Random](System.md#random)
- [ResourceManager](System.md#resourcemanager)
- [Singleton](System.md#singleton)
- [ThreadComponentHandler](System.md#threadcomponenthandler)
- [Time](System.md#time)
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

**removableComponents**  
```cpp
std::vector<T*> removableComponents;
```

**mutex**  
```cpp
std::mutex mutex;
```

### Functions:
**Protected:**  
**BaseComponentHandler**  
```cpp
BaseComponentHandler();
```

**RefreshComponents**  
It removes the `removableComponents` and adds the `newComponents` to the `components`.
After this, it clears the `newComponents` and the `removableComponents`.
```cpp
virtual void RefreshComponents();
```
**RemoveItem**  
It will remove the item from the new colliders or they will add the item into removable colliders.  
It is used by Remove function.
```cpp
void RemoveItem(T* component);
```

**Public:**  
**Add**  
If the isThreadSafe is true, the mutex will be used.
```cpp
virtual void Add(T* component, bool isThreadSafe);
```

**Remove**  
If the isThreadSafe is true, the mutex will be used.
```cpp
virtual void Remove(T* component, bool isThreadSafe);
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
[CameraTest.cpp](../../Learning2DEngineTest/System/CameraTest.cpp)

### Description:
The `Camera` contains the camera settings, which the developer can transform the view of the users.
The shaders can use its projection and its view matrix.
The developer can change the Camera's resolution, position and rotation too.  
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
Note: the `Render::RenderManager`'s resolution and the `System::Camera`'s resolution are not same.  
The `Render::RenderManager`'s resolution is the real resolution, how the code render the game.  
The `System::Camera`'s resolution is like a coordinate system,
which the developer can use where they want to put the gameobjects.  
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
class ComponentManager final : public Singleton<ComponentManager>
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

**isThreadSafe**  
```cpp
bool isThreadSafe;
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

**SetUpdateMaxComponentPerThread**  
If it is bigger then 0, than every component handlers and the `GameObjectManager`
will be thread safe.  
But if it is 0, the thread safe will not be turn off automatically.  
```cpp
void SetUpdateMaxComponentPerThread(unsigned int value);
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

**SetLateUpdateMaxComponentPerThread**  
If it is bigger then 0, than every component handlers and the `GameObjectManager`
will be thread safe.  
But if it is 0, the thread safe will not be turn off automatically.  
```cpp
void SetLateUpdateMaxComponentPerThread(unsigned int value);
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

**SetMaxColliderPerThread**  
If it is bigger then 0, than every component handlers and the `GameObjectManager`
will be thread safe.  
But if it is 0, the thread safe will not be turn off automatically.  
```cpp
void SetMaxColliderPerThread(unsigned int value);
```

**IsRendererExist**  
```cpp
inline bool IsRendererExist(const std::string& id);
```

**AddRenderer**  
```cpp
inline void AddRenderer(const std::string& id, Render::IRenderer* renderer);
```

**RemoveRenderer**  
```cpp
inline void RemoveRenderer(const std::string& id);
```

**AddRenderData**  
```cpp
inline void AddRenderData(Render::RendererMode mode, const std::string& id, Render::RenderData* data, int layer);
```

**ChangeRenderLayer**  
```cpp
inline void ChangeRenderLayer(Render::RenderData* data, int newLayer);
```

**RemoveRenderData**  
```cpp
inline void RemoveRenderData(Render::RenderData* data);
```

**SetDataToRenderers**  
It sets the data to the renderers, before the Render() and LateRender() functions are called.
```cpp
inline void SetDataToRenderers();
```

**Render**  
```cpp
inline void Render();
```

**LateRender**  
```cpp
inline void LateRender();
```

**SetThreadSafe**  
It set the thread safe mode the `ComponentManager` and the `GameObjectManager` too.
```cpp
inline void SetThreadSafe(bool value);
```

**GetThreadSafe**  
```cpp
inline bool GetThreadSafe();
```

**Clear**  
Clear all handlers.
```cpp
void Clear();
```

##
## Cursor
### Source Code:
[Cursor.h](../../Learning2DEngine/Learning2DEngine/System/Cursor.h)  

### Description: 
It contains the mouse buttons, cursor position, scroll and the
cursor is in the window.

### Header:
```cpp
struct Cursor {
	InputStatus mouseButtons[L2DE_MOUSE_BUTTON_NUMBER];
	glm::vec2 position;
	glm::vec2 scroll;
	bool isInWindow;
};
```

### Macros:
**L2DE_MOUSE_BUTTON_NUMBER**  
The number of mouse buttons.  
Its value is 8.

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
10. Destroy Marked GameObjects

### Header:
```cpp
class Game : private IKeyboardRefresher, private ICursorRefresher, public protected::IResolutionRefresher
{...}
```

### Macros:
**L2DE_KEYBOARD_BUTTON_NUMBER**  
The number of keyboard buttons.
Its value is 512.

### Variables:
**Private:**  
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
```cpp
EventSystem::KeyboardEventItem keyboardEventItem;
```

**mouseButtonEventItem**  
```cpp
 EventSystem::MouseButtonEventItem mouseButtonEventItem;
```

**cursorPositionEventItem**  
```cpp
EventSystem::CursorPositionEventItem cursorPositionEventItem;
```

**cursorEnterEventItem**  
```cpp
EventSystem::CursorEnterEventItem cursorEnterEventItem;
```

**scrollEventItem**  
```cpp
EventSystem::ScrollEventItem scrollEventItem;
```

**resolutionEventItem** 
```cpp
EventSystem::ResolutionEventItem resolutionEventItem;
```

**keyboardButtons**  
This array contains, which keyboard button is up, down or hold.
```cpp
static InputStatus keyboardButtons[L2DE_KEYBOARD_BUTTON_NUMBER];
```

**cursor**  
```cpp
static Cursor cursor;
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

**FixCursor**  
The *glfwPollEvents* does have InputStatus::KEY_HOLD for Mouse buttons.
Moreover it doesn't refresh the scroll values to 0.0f.  
So this function do it.
```cpp
void FixCursor();
```

**FixKeyboardButtons**  
The *glfwPollEvents* does not refresh the data on every frame.
That's why this function update the `InputStatus::KEY_DOWN`
to `InputStatus::KEY_HOLD`.
```cpp
void FixKeyboardButtons();
```

**RefreshKeyboard**  
The `Game` subscribes for keyboard events and the `RenderManager` call
this function by an event.
```cpp
void RefreshKeyboard(int key, int scancode, int action, int mode) override;
``` 

**RefreshMouseButton**  
The `Game` subscribes for mouse button events and the `RenderManager` call
this function by an event.
```cpp
void RefreshMouseButton(int button, int action, int mods) override;
``` 

**RefreshCursorPosition**  
The `Game` subscribes for cursor position events and the `RenderManager` call
this function by an event.
```cpp
void RefreshCursorPosition(double xpos, double ypos) override;
``` 

**RefreshCursorInWindows**  
The `Game` subscribes for cursor enter and the `RenderManager` call
this function by an event.
```cpp
void RefreshCursorInWindows(bool entered) override;
``` 

**RefreshScroll**  
The developer should not use this function.
The `Game` subscribes for scroll events and the `RenderManager` call
this function by an event.
```cpp
void RefreshScroll(double xoffset, double yoffset) override;
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

**RefreshResolution**  
If this function is override, it must call the Game::RefreshResolution(const Resolution& resolution)
in the first line.  
The `Game` subscribe for (game) screen resolution events
and the `RenderManager` call this function by an event.
```cpp
virtual void RefreshResolution(const Render::Resolution& resolution) override;
``` 

**Public:**  
**~Game**  
```cpp
virtual ~Game();
```  

**InitWithRender**  
It initializes the `RenderManager`. After that, the `Init()` will be called.
```cpp
void InitWithRender(int majorRenderVersion, int minorRenderVersion, Render::Resolution resolution, const char* title, Render::WindowType windowType);
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

**GetKeyboardButtonStatus**  
It returns the status of a keyboard button.  
```cpp
static InputStatus GetKeyboardButtonStatus(int key);
``` 

**GetMouseButtonStatus**  
It returns the status of a mouse button.  
```cpp
static InputStatus GetMouseButtonStatus(int key);
``` 

**GetCursorPosition**  
It returns the cursor's position.  
```cpp
static glm::vec2 GetCursorPosition();
``` 

**IsCursorInWindow**  
It returns, that the cursor is in the window or not.  
```cpp
static bool IsCursorInWindow();
``` 

**GetScroll**  
It returns the scroll's position. 
```cpp
static glm::vec2 GetScroll();
``` 

##
## GameObject
### Source Code:
[GameObject.h](../../Learning2DEngine/Learning2DEngine/System/GameObject.h)  
[GameObjectTest.cpp](../../Learning2DEngineTest/System/GameObjectTest.cpp)

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
**Private:**  
**GameObject**  
```cpp
GameObject(bool isActive = true);
```
```cpp
GameObject(const Transform& transform, bool isActive = true);
```

**Public:**  
**~GameObject()**  
```cpp
~GameObject() = default;
```

**Destroy()**  
It calls Destroy() function of the current gameobject's components
and delete them.
```cpp
void Destroy();
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
playerController = player->AddComponent<PlayerController>("paddle");
```
```cpp
template <class TComponent, class ...Params>
TComponent* AddComponent(Params&&... params);
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

**Deleted:**  
```cpp
GameObject(const GameObject&) = delete;
```
```cpp
GameObject& operator=(const GameObject&) = delete;
```

##
## GameObjectManager
### Source Code:
[GameObjectManager.h](../../Learning2DEngine/Learning2DEngine/System/GameObjectManager.h)  
[GameObjectManager.cpp](../../Learning2DEngine/Learning2DEngine/System/GameObjectManager.cpp)

### Description:
The `GameObjectManager` manages the `GameObjects` in the Engine.  
With this class, the `Game` destroy the marked `GameObjects` at end of the frame and all `GameObjects`
when we close the window.

### Header:
```cpp
class GameObjectManager : public Singleton<GameObjectManager>
{...}
```

### Variables:
**Private:**  
**gameObjects**  
```cpp
std::vector<GameObject*> gameObjects;
```

**removableGameObjects**  
```cpp
std::vector<GameObject*> removableGameObjects;
```

**addMutex**  
```cpp
std::mutex addMutex;
```

**removeMutex**  
```cpp
std::mutex removeMutex;
```

**isThreadSafe**  
```cpp
bool isThreadSafe;
```

### Functions:
**Private:**  
**GameObjectManager**  
```cpp
GameObjectManager();
```

**Public:**  
**CreateGameObject**  
```cpp
GameObject* CreateGameObject(bool isActive = true);
```
```cpp
GameObject* CreateGameObject(const Transform& transform, bool isActive = true);
```

**DestroyGameObject**  
The `GameObject` and its components will be destroyed.  
If the developer give a `Component`, it will destroy the `GameObject` of the `Component`.  
That's why all other `Components`,which the `GameObject` has, will be destroyed.  
`GameObject` will be inactive immediately, but it will be destroyed just at end of the frame only.
```cpp
void DestroyGameObject(GameObject* gameObject);
```
```cpp
void DestroyGameObject(Component* component);
```

**DestroyMarkedGameObjects**  
It destroys those `GameObjects` immediately, which was marked with
DestroyGameObject function.  
The `Game` calls this function end of the frame.  
```cpp
void DestroyMarkedGameObjects();
```

**DestroyAllGameObjects**  
It destroys all `GameObjects` immediately.  
The `Game` calls this function in its Terminate function. 
```cpp
void DestroyAllGameObjects();
```

**SetThreadSafe**  
```cpp
inline void SetThreadSafe(bool value);
```

**GetThreadSafe**  
```cpp
inline bool GetThreadSafe();
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
virtual ~IComponentHandler() = default;
```

**Clear**  
```cpp
virtual void Clear() = 0;
```

**Run**  
```cpp
virtual void Run() = 0;
```

##
## ICursorRefresher
### Source Code:
[ICursorRefresher.h](../../Learning2DEngine/Learning2DEngine/System/ICursorRefresher.h)

### Description:
It is a little interface, which can be used for mouse/cursor events and it is wrapable
into `CursorEnterEventItem`, `CursorPositionEventItem`, `MouseButtonEventItem` or `ScrollEventItem`.

### Header:
```cpp
class ICursorRefresher
{...}
```

### Functions:
**Public:**  
**~ICursorRefresher**  
```cpp
virtual ~ICursorRefresher();
```

**RefreshMouseButton**  
```cpp
virtual void RefreshMouseButton(int button, int action, int mods) = 0;
```

**RefreshCursorPosition**  
```cpp
virtual void RefreshCursorPosition(double xpos, double ypos) = 0;
```

**RefreshCursorInWindows**  
```cpp
virtual void RefreshCursorInWindows(bool entered) = 0;
```

**RefreshScroll**  
```cpp
virtual void RefreshScroll(double xoffset, double yoffset) = 0;
```

##
## IKeyboardRefresher
### Source Code:
[IKeyboardRefresher.h](../../Learning2DEngine/Learning2DEngine/System/IKeyboardRefresher.h)

### Description:
It is a little interface, which can be used for keyboard events and it is wrapable into `KeyboardEventItem`.

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
It is an enum, which contains the 3 phase of the buttons.  
It is not enum class, because it can be useful, that the value is true if the input is down or hold in a "if" statement.

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

### Header:
```cpp
class LateUpdaterComponent : public BaseLateUpdaterComponent
{...}
```
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
class LateUpdaterComponentHandler : public ThreadComponentHandler<BaseLateUpdaterComponent>
{...}
```

### Functions:
**Protected:**  
**RunPart** 
It iterates on the components vector in [startIndex, endIndex) range.  
If the component and its gameobject is active, its LateUpdate() function will be called.
```cpp
void RunPart(size_t startIndex, size_t endIndex) override;
```

**Public:**  
**LateUpdaterComponentHandler**  
```cpp
LateUpdaterComponentHandler();
```

##
## Math
### Source Code:
[Math.h](../../Learning2DEngine/Learning2DEngine/System/Math.h)  
[MathTest.cpp](../../Learning2DEngineTest/System/MathTest.cpp)

### Description:
It contains some Math functions.

### Header:
```cpp
struct Math
{...}
```

### Functions:
**Public:**  
**FlipByX**  
Horizontally flips the matrix by X axis.
```cpp
static inline glm::mat4x2 FlipByX(const glm::mat4x2& matrix);
```

**FlipByY**  
Vertically flips the matrix by Y axis.
```cpp
static inline glm::mat4x2 FlipByY(const glm::mat4x2& matrix);
```

##
## Random
### Source Code:
[Random.h](../../Learning2DEngine/Learning2DEngine/System/Random.h)  
[Random.cpp](../../Learning2DEngine/Learning2DEngine/System/Random.cpp)  
[RandomTest.cpp](../../Learning2DEngineTest/System/RandomTest.cpp)

### Description:
It is a static random number generator.

### Header:
```cpp
class Random
{...}
```

### Functions:
**Private:**  
**Random**  
```cpp
Random();
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
class ResourceManager : public Singleton<ResourceManager>
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

**Public:**  
**LoadShaderFromFile**  
It creates a `Shader` from files.
```cpp
 Render::Shader& LoadShaderFromFile(const std::string& id, const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath = nullptr);
```

**LoadShader**  
It creates a `Shader` from strings.
```cpp
Render::Shader& LoadShader(const std::string& id, const char* vertexText, const char* fragmentText, const char* geometryText = nullptr);
```

**GetShader**  
It returns a `Shader`. If the shader does not exist,
it will return an uninitialized `Shader`.  
It is recommended to use the `IsShaderExist()`.
```cpp
Render::Shader& GetShader(const std::string& id);
```

**IsShaderExist**  
It returns true if the `Shader` exist.
```cpp
bool IsShaderExist(const std::string& id);
```

**DestroyShader**  
It destroys the `Shader`.
```cpp
void DestroyShader(const std::string& id);
```

**LoadTextureFromFile**  
It creates a `Texture2D` from files.  
If the id is used, it will return the old one.
```cpp
Render::Texture2D& LoadTextureFromFile(const std::string& id, const char* filePath, const Render::Texture2DSettings& settings);
```

**GetTexture**  
It returns a `Texture2D`. If the texture does not exist,
it will return an uninitialized `Texture2D`.
It is recommended to use the `IsTextureExist()`.
```cpp
Render::Texture2D& GetTexture(const std::string& id);
```

**IsTextureExist**  
It returns true if the `Texture2D` exist.
```cpp
bool IsTextureExist(const std::string& id);
```

**DestroyTexture**  
It destroys the `Texture2D`.
```cpp
void DestroyTexture(const std::string& id);
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
## ThreadComponentHandler
### Source Code:
[ThreadComponentHandler.h](../../Learning2DEngine/Learning2DEngine/System/ThreadComponentHandler.h)  

### Description: 
It is a component handler, which can use threads.

### Header:
```cpp
template<class T>
class ThreadComponentHandler : public BaseComponentHandler<T>
{...}
```

### Variables:
**Protected:**  
**maxComponentPerThread**  
If it is 0, the class will not use threads.
```cpp
unsigned int maxComponentPerThread;
```

**threads**  
```cpp
std::vector<std::thread> threads;
```

### Functions:
**Protected:**  
**RunPart**  
The threads will call this function with the component vector indexes.
```cpp
virtual void RunPart(size_t startIndex, size_t endIndex) = 0;
```

**RunOnThreads**  
```cpp
void RunOnThreads();
```

**Public:**  
**ThreadComponentHandler**  
```cpp
ThreadComponentHandler();
```

**Run**  
Firstly it refresh the component vector and it will call the RunPart and/or RunOnThreads functions.  
How many threads will run, it is depend on the number of components and maxComponentPerThread.
```cpp
virtual void Run() override;
```

**SetMaxComponentPerThread**  
If it is 0, the class will not use threads.
```cpp
inline void SetMaxComponentPerThread(unsigned int value);
```

**GetMaxComponentPerThread**  
If it is 0, the class will not use threads.
```cpp
inline unsigned int GetMaxComponentPerThread();
```

##
## Time
### Source Code:
[Time.h](../../Learning2DEngine/Learning2DEngine/System/Time.h)  
[Time.cpp](../../Learning2DEngine/Learning2DEngine/System/Time.cpp)  

### Description: 
It contains the time functions.

### Header:
```cpp
class Time
{...}
```

### Macros:
**L2DE_TIME_SCALE_DEFAULT**  
The default value of the timeScale.
Its value is 1.0f.

### Variables:
**Private:**  
**deltaTime**  
It is multiplied by timeScale.  
Before the first frame, it is 0.0f.
```cpp
static float deltaTime;
```

**timeScale**  
```cpp
static float timeScale;
```

**lastTime**  
```cpp
static float lastTime;
```

**isInited**  
```cpp
static bool isInited;
```

### Functions:
**Private:**  
**Time**  
```cpp
Time() = default;
```

**~Time**  
```cpp
~Time() = default;
```

**CalcDeltaTime**  
```cpp
static void CalcDeltaTime();
```

**Public:**  
**ResetTimeScale**  
```cpp
static inline void ResetTimeScale();
```

**SetTimeScale**  
```cpp
static inline void SetTimeScale(float value);
```

**GetTimeScale**  
```cpp
static inline float GetTimeScale();
```

**GetDeltaTime**  
```cpp
static inline float GetDeltaTime();
```

##
## Transform
### Source Code:
[Transform.h](../../Learning2DEngine/Learning2DEngine/System/Transform.h)  
[TransformTest.cpp](../../Learning2DEngineTest/System/TransformTest.cpp)

### Description: 
It contains the position the scale and the rotation.  
Every `GameObject` has a `Transform`.

### Header:
```cpp
struct Transform
{...}
```

### Variables:
**Private:**  
**position**  
```cpp
glm::vec2 position;
```

**scale**  
```cpp
glm::vec2 scale;
```

**rotation**  
```cpp
float rotation;
```

**isModified**  
If it is true, the modelMatrix will be recalculated.
```cpp
bool isModified;
```

**modelMatrix**  
```cpp
glm::mat4 modelMatrix;
```

### Functions:
**Private:**  
**CalculateModelMatrix**  
```cpp
glm::mat4 CalculateModelMatrix() const;
```

**Public:**  
**Transform**  
```cpp
Transform(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f);
```

**GetPosition**  
```cpp
inline glm::vec2 GetPosition() const;
```

**SetPosition**  
```cpp
void SetPosition(const glm::vec2& newPosition);
```

**AddPosition**  
```cpp
void AddPosition(const glm::vec2& deltaPosition);
```

**GetScale**  
```cpp
inline glm::vec2 GetScale() const;
```

**SetScale**  
```cpp
void SetScale(const glm::vec2& newScale);
```

**AddScale**  
```cpp
void AddScale(const glm::vec2& deltaScale);
```

**GetRotation**  
```cpp
inline float GetRotation() const;
```

**SetRotation**  
```cpp
void SetRotation(float newRotation);
```

**AddRotation**  
```cpp
void AddRotation(float deltaRotation);
```

**GetModelMatrix**  
If the isModified is true, the non const version save the new calculated model matrix,
that's why it will not recalculate the model matrix again.  
In same situation the const version will always recalculate the model matrix.
```cpp
const glm::mat4& GetModelMatrix();
```
```cpp
glm::mat4 GetModelMatrix() const;
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

### Header:
```cpp
class UpdaterComponent : public BaseUpdaterComponent
{...}
```

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
class UpdaterComponentHandler : public ThreadComponentHandler<BaseUpdaterComponent>
{...}
```

### Functions:
**Protected:**  
**RunPart** 
It iterates on the components vector in [startIndex, endIndex) range.  
If the component and its gameobject is active, its Update() function will be called.
```cpp
void RunPart(size_t startIndex, size_t endIndex) override;
```

**Public:**  
**UpdaterComponentHandler**  
```cpp
UpdaterComponentHandler();
```
