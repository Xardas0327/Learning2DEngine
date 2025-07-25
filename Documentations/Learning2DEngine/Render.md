# Render
- [BaseColorDynamicData](Render.md#basecolordynamicdata)
- [BaseMultiRenderer](Render.md#basemultirenderer)
- [BaseRenderer](Render.md#baserenderer)
- [BaseRendererComponent](Render.md#baserenderercomponent)
- [BlendFuncFactor](Render.md#blendfuncfactor)
- [IRenderer](Render.md#irenderer)
- [IResolutionRefresher](Render.md#iresolutionrefresher)
- [MSAA](Render.md#msaa)
- [MultiSpriteDynamicData](Render.md#multispritedynamicdata)
- [MultiSpriteRenderer](Render.md#multispriterenderer)
- [PostProcessEffect](Render.md#postprocesseffect)
- [RenderData](Render.md#renderdata)
- [RendererComponent](Render.md#renderercomponent)
- [RendererComponentHandler](Render.md#renderercomponenthandler)
- [RendererMode](Render.md#renderermode)
- [RenderManager](Render.md#rendermanager)
- [Resolution](Render.md#resolution)
- [Shader](Render.md#shader)
- [ShaderConstant](Render.md#shaderconstant)
- [SimpleSpriteRenderComponent](Render.md#simplespriterendercomponent)
- [SimpleSpriteRenderer](Render.md#simplespriterenderer)
- [SpriteRenderComponent](Render.md#spriterendercomponent)
- [SpriteRenderData](Render.md#spriterenderdata)
- [Texture2D](Render.md#texture2d)
- [Texture2DContainer](Render.md#texture2dcontainer)
- [Texture2DSettings](Render.md#texture2dsettings)

##
## BaseColorDynamicData
### Source Code:
[BaseColorDynamicData.h](../../Learning2DEngine/Learning2DEngine/Render/BaseColorDynamicData.h)  

### Description:
It contains the dynamic data of the Debug colliders.

### Header:
```cpp
struct BaseColorDynamicData
{
	float modelMatrix[16];
	float color[4];
};
```

##
## BaseMultiRenderer
### Source Code:
[BaseMultiRenderer.h](../../Learning2DEngine/Learning2DEngine/Render/BaseMultiRenderer.h)  

### Description:
It expands the `BaseRenderer` with multi renderer variables, functions.

### Header:
```cpp
template<class TDynamicData>
class BaseMultiRenderer : public BaseRenderer
{...}
```

### Variables:
**Protected:**  
**vboDynamic**  
It contains the uploaded model matrices, colors and similar data of the renderable objects.
```cpp
GLuint vboDynamic;
```

**maxObjectSize**  
The size of the vboDynamic, dynamicData.
```cpp
size_t maxObjectSize;
```

**dynamicData**  
It is array, which contains the model matrices, colors and similar data of the renderable objects, before the upload.  
Note: its size is the maxObjectSize, so it will be reallocated only, when the maxObjectSize is changed.
```cpp
TDynamicData* dynamicData;
```

### Functions:
**Protected:**  
**BaseMultiRenderer**  
```cpp
BaseMultiRenderer();
```

**DestroyObject**  
The Destroy() call it with or without mutex.
```cpp
virtual void DestroyObject();
```

**CalcDynamicDataSize**  
If the current buffers size is not enough or it is bigger twice than the maxDynamicSize,
it will reallocate the buffers.  
It allocates 20% more space in the buffer, so that it does not have to allocate again 
if there are some dynamic renderers.
```cpp
virtual void CalcDynamicDataSize(size_t maxDynamicSize);
```

##
## BaseRenderer
### Source Code:
[BaseRenderer.h](../../Learning2DEngine/Learning2DEngine/Render/BaseRenderer.h)  

### Description:
It contains some basic funcitions and variable for the renderers.

### Header:
```cpp
class BaseRenderer : public IRenderer
{...}
```

### Variables:
**Protected:**  
**shader**  
```cpp
Shader* shader;
```

**vao**  
```cpp
GLuint vao;
```

**ebo**  
```cpp
GLuint ebo;
```

**vbo**  
```cpp
GLuint vbo;
```

### Functions:
**Protected:**  
**BaseRenderer**  
```cpp
BaseRenderer();
```

**InitShader**  
```cpp
virtual void InitShader() = 0;
```

**InitVao**  
```cpp
virtual void InitVao() = 0;
```

**DestroyObject**  
The Destroy() call it with or without mutex.
```cpp
virtual void DestroyObject();
```

**Public:**  
**Init**  
If the `ComponentManager` is in thread save mode, it will call the `RenderManager`'s mutex,
before InitShader and InitVao functions.
```cpp
void Init() override;
```

**Destroy**  
If the `ComponentManager` is in thread save mode, it will call the `RenderManager`'s mutex,
before DestroyObject function.
```cpp
void Destroy() override;
```

##
## BaseRendererComponent
### Source Code:
[BaseRendererComponent.h](../../Learning2DEngine/Learning2DEngine/Render/BaseRendererComponent.h)

### Header:
```cpp
template<class TRenderData, class TRenderer>
class BaseRendererComponent : public virtual System::Component
{...}
```
### Description:
It has some basic funcionality, which is essential for rendering, but this is a support
class only, please use `RendererComponent` instead of this.  
The TRenderData should be a class, which is inhereted from IRenderData.  
The TRenderer should be a class, which is inhereted from IRenderer.  
Please check for more info about `RendererComponent` and `System::Component`.

### Header:
```cpp
template<class TRenderData, class TRenderer>
class BaseRendererComponent : public virtual System::Component
{...}
```

### Variables:
**Private:**  
**layer**  
This is the order of the rendering. The lower number means, this renderer will be called ealier.
```cpp
int layer;
```

**Public:**  
**data**  
This one contains the data, which will be used in the rendering.
```cpp
TRenderData data;
```

### Functions:
**Protected:**  
**BaseRendererComponent**  
```cpp
template <class ...TRenderDataParams>
BaseRendererComponent(System::GameObject* gameObject, int layer = 0, TRenderDataParams&&... renderDataParams);
```

**GetId**  
It returns the id, which is used to find the data and renderer pairs by the `RendererComponentHandler`.
```cpp
virtual const std::string& GetId() const = 0;
```

**GetInitedRenderer**  
```cpp
virtual TRenderer* GetInitedRenderer() = 0;
```

**DestroyRenderer**  
```cpp
virtual void DestroyRenderer() = 0;
```

**Public:**  
**SetLayer**  
```cpp
virtual void SetLayer(int value);
```

**GetLayer**  
```cpp
inline int GetLayer() const;
```

##
## BlendFuncFactor
### Source Code:
[Blend.h](../../Learning2DEngine/Learning2DEngine/Render/Blend.h)

### Description:
It is a pair, where the developer have to use the the OpenGL Blend's constants.  
The first is the source factor.  
The second is the destination factor.

### Header:
```cpp
typedef std::pair<unsigned int, unsigned int> BlendFuncFactor;
```

##
## IRenderer
### Source Code:
[IRenderer.h](../../Learning2DEngine/Learning2DEngine/Render/IRenderer.h)

### Description:
It is am interface for the renderers.

### Header:
```cpp
struct IRenderer
{...}
```

### Functions:
**Public:**  
**~IRenderer**  
```cpp
virtual ~IRenderer() = default;
```

**Init**  
```cpp
virtual void Init() = 0;
```

**Destroy**  
```cpp
virtual void Destroy() = 0;
```

**SetData**  
Note: the int is the layer.
```cpp
virtual void SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData) = 0;
```

**Draw**  
It draws those objects, which was added with SetData and they are on the selected mode and layer.
```cpp
virtual void Draw(RendererMode rendererMode, int layer) = 0;
```

##
## IResolutionRefresher
### Source Code:
[IResolutionRefresher.h](../../Learning2DEngine/Learning2DEngine/Render/IResolutionRefresher.h)

### Description:
It is a little interface, which the developer can use to wrap a class into `ResolutionEventItem`.

### Header:
```cpp
class IResolutionRefresher
{...}
```

### Functions:
**Public:**  
**~IResolutionRefresher**  
```cpp
virtual ~IResolutionRefresher();
```

**RefreshResolution**  
```cpp
virtual void RefreshResolution(const Resolution& resolution) = 0;
```

##
## MSAA
### Source Code:
[MSAA.h](../../Learning2DEngine/Learning2DEngine/Render/MSAA.h)  
[MSAA.cpp](../../Learning2DEngine/Learning2DEngine/Render/MSAA.cpp)

### Description:
Multisample anti-aliasing for rendering.

### Header:
```cpp
class MSAA
{...}
```

### Variables:
**Private:**  
**fbo**  
```cpp
unsigned int fbo;
```

**rbo**  
```cpp
unsigned int rbo;
```

**sampleNumber**  
```cpp
unsigned int sampleNumber;
```

**resolution**  
```cpp
Resolution resolution;
```

### Functions:
**Public:**  
**MSAA**  
```cpp
MSAA();
```

**~MSAA**  
```cpp
~MSAA() = default;
```

**Init**  
It initializes the `MSAA` and its frame buffer.
```cpp
void Init(unsigned int sampleNumber, Resolution resolution);
```

**Destroy**  
It destroys the `MSAA`.
```cpp
void Destroy();
```

**StartRender**  
It binds the `MSAAs` frame buffer. Plus it clears the buffer if the `useClear` is true.
```cpp
void StartRender(bool useClear = true);
```

**EndRender**  
It copies the `MSAA` buffer data to result framebuffer and it convert the `MSAA` resolution to `resultResolution`.
```cpp
void EndRender(unsigned int resultFbo, Resolution  resultResolution);
```

**GetId**  
It returns the frame buffor object id.
```cpp
inline unsigned int GetId();
```

**GetSampleNumber**  
It returns the sample number.
```cpp
inline unsigned int GetSampleNumber();
```

##
## MultiSpriteDynamicData
### Source Code:
[MultiSpriteDynamicData.h](../../Learning2DEngine/Learning2DEngine/Render/MultiSpriteDynamicData.h)  

### Description:
It contains the dynamic data of the `MultiSpriteRenderer`.  
Note: the textureId and the isUseCameraView are float, because we sent it to vertex shader,
but textureId will be converted to int and the isUseCameraView will be converted to bool.

### Header:
```cpp
struct MultiSpriteDynamicData : public BaseColorDynamicData
{
    float textureId;
    float isUseCameraView;
};
```

##
## MultiSpriteRenderer
### Source Code:
[MultiSpriteRenderer.h](../../Learning2DEngine/Learning2DEngine/Render/MultiSpriteRenderer.h)  
[MultiSpriteRenderer.cpp](../../Learning2DEngine/Learning2DEngine/Render/MultiSpriteRenderer.cpp)

### Description:
A sprite renderer, which has multi instancing support.  
It has better performance, than the `SimpleSpriteRenderer`,
when the object has a lot of instances in a layer.  
Note: The projection and the view matrix came from Game::mainCamera.

### Header:
```cpp
class MultiSpriteRenderer : public BaseMultiRenderer<MultiSpriteDynamicData>, public System::Singleton<MultiSpriteRenderer>
{...}
```

### Variables:
**Private:**  
**spriteRenderData**  
When the SetData is called, the renderData will be converted to this format.  
The int is the layer.  
The third map's key is the texture id and the value is the `SpriteRenderData` vector, which use that texture.  
If the sprite doesn't use texture, the key is 0.
```cpp
std::map <RendererMode, std::map<int, std::map<GLuint, std::vector<SpriteRenderData*>>>> spriteRenderData;
```

### Functions:
**Private:**  
**MultiSpriteRenderer**  
```cpp
MultiSpriteRenderer();
```

**InitShader**  
```cpp
void InitShader() override;
```

**InitVao**  
```cpp
void InitVao() override;
```

**DestroyObject**  
The Destroy() call it with or without mutex.
```cpp
void DestroyObject() override;
```

**Public:**  
**SetData**  
It allocates 20% more space in the buffer, so that it does not have to allocate again 
if there are some dynamic renderers.  
Note: the int is the layer.
```cpp
void SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData) override;
```

**Draw**  
It draws those objects, which was added with SetData and they are on the selected mode and layer.
```cpp
void Draw(Render::RendererMode rendererMode, int layer) override;
```

##
## PostProcessEffect
### Source Code:
[PostProcessEffect.h](../../Learning2DEngine/Learning2DEngine/Render/PostProcessEffect.h)  
[PostProcessEffect.cpp](../../Learning2DEngine/Learning2DEngine/Render/PostProcessEffect.cpp)

### Description:
Post Process Effect for rendering.

### Header:
```cpp
class PostProcessEffect
{...}
```

### Variables:
**Private:**  
**defaultShader**  
It is a simple shader,
which just render the image without any effects.
```cpp
Shader* defaultShader;
```

**effectShader**  
```cpp
const Shader* effectShader;
```

**texture**  
This `texture` is not in the `ResourceManager`,
the `PostProcessEffect` has the full controll on it.
```cpp
Texture2D texture;
```

**vao**  
```cpp
unsigned int vao;
```

**vbo**  
```cpp
unsigned int vbo;
```

**ebo**  
```cpp
unsigned int ebo;
```

**fbo**  
```cpp
unsigned int fbo;
```

### Functions:
**Private:**  
**InitVao**  
```cpp
void InitVao();
```

**InitDefaultShader**  
```cpp
void InitDefaultShader();
```

**Public:**  
**PostProcessEffect**  
```cpp
PostProcessEffect();
```

**~PostProcessEffect**  
```cpp
~PostProcessEffect();
```

**Init**  
It initializes PostProcessEffect and its frame buffer and texture.
```cpp
void Init(Resolution resolution);
```

**Destroy**  
It destroys the PostProcessEffect.
```cpp
void Destroy();
```

**StartRender**  
It binds the `PostProcessEffects` frame buffer.
Plus it clears the buffer if the useClear is true.
```cpp
void StartRender(bool useClear = true);
```

**EndRender**  
It binds the default frame buffer.
```cpp
void EndRender();
```

**Render**  
It renders the `texture` of the `PostProcessEffect`.
```cpp
void Render();
```

**SetShader**  
It set the `effectShader`.
```cpp
void SetShader(const Shader& shader);
```

**ClearShader**  
`PostProcessEffect` start to use to the `defaultShader`;
```cpp
void ClearShader();
```

**GetFrameBufferId**  
It returns the frame buffor object id.
```cpp
inline unsigned int GetFrameBufferId();
```

##
## RenderData
### Source Code:
[RenderData.h](../../Learning2DEngine/Learning2DEngine/Render/RenderData.h)

### Description:
It contains the data, which will be used in the rendering.

### Header:
```cpp
struct RenderData
{...}
```

### Variables:
**Public:**  
**component**  
It is a reference to the `System::Component` object, that `RendererComponentHandler` can ask, that
the component and the gameobject are active or not.
```cpp
const System::Component* const component;
```

### Functions:
**Public:**  
**RenderData**  
```cpp
RenderData(const System::Component* component);
```

**~RenderData**  
```cpp
virtual ~RenderData() = default;
```

##
## RendererComponent
### Source Code:
[RendererComponent.h](../../Learning2DEngine/Learning2DEngine/Render/RendererComponent.h)

### Description:
It is a class, which is inherited from `BaseRendererComponent`.  
The developer have to inherit from this class, if they want to render something.  
It has 2 modes, RendererMode::Render and RendererMode::LateRender.  
It uses static variables to count how many GameObject initialized it.
That's why it will destroy renderer only if the reference number is 0,
otherway it will decrease the reference number.  
The Render is before the LateRender and the Render can have anti-aliasing
and/or post process effects.  
Note: The layer of the Render is the order in the Render only.
So if a LateRender has a lower layer number, it will be still rendered after the Render.   
Please check for more info about `System::Component` and `BaseRendererComponent`.

### Header:
```cpp
template<class TRenderData, class TRenderer>
class RendererComponent : public BaseRendererComponent<TRenderData, TRenderer>
{...}
```

### Variables:
**Protected:**  
**refrenceNumber**  
It is counted, that how many RendererComponent<TRenderData, TRenderer> exist.
```cpp
static int refrenceNumber;
```

**mutex**  
```cpp
static std::mutex mutex;
```

**Public:**  
**mode**  
```cpp
const RendererMode mode;
```

### Functions:
**Protected:**  
**RendererComponent**  
```cpp
template <class ...TRenderDataParams>
RendererComponent(System::GameObject* gameObject, RendererMode mode, int layer = 0, TRenderDataParams&&... renderDataParams);
```

**InitObject**  
The Init() call it with or without mutex.
```cpp
void InitObject();
```

**DestroyObject**  
The Destroy() call it with or without mutex.
```cpp
void DestroyObject();
```

**Init**  
If this function is override, it must call the RendererComponent::Init() in the first line.
```cpp
virtual void Init() override;
```

**Destroy**  
If this function is override, it must call the RendererComponent::Destroy() in the first line.
```cpp
virtual void Destroy() override;
```

**Public:**  
**SetLayer**  
```cpp
virtual void SetLayer(int value) override;
```

##
## RendererComponentHandler
### Source Code:
[RendererComponentHandler.h](../../Learning2DEngine/Learning2DEngine/Render/RendererComponentHandler.h)  
[RendererComponentHandler.cpp](../../Learning2DEngine/Learning2DEngine/Render/RendererComponentHandler.cpp)  

### Description:
It can handle the `IRenderer` and the `RenderData` objects.  
The `ComponentManager` has 2 from it.
One for those RenderComponents, which has RenderMode::RENDER
and one for those ones, which has RenderMode::LATERENDER.

### Header:
```cpp
class RendererComponentHandler final
{...}
```

### Variables:
**Private:**  
**renderers**  
```cpp
std::map<std::string, IRenderer*> renderers;
```

**renderData**  
The string is the render id.  
The int is the layer.
```cpp
std::map<std::string, std::map<RendererMode, std::map<int, std::vector<RenderData*>>>> renderData;
```

**renderDataMapping**  
It helps for find the data faster, when the layer is changed.
```cpp
std::map<RenderData*, std::tuple<std::string, RendererMode, int>> renderDataMapping;
```

**rendererMutex**  
```cpp
std::mutex rendererMutex;
```

**dataMutex**  
```cpp
std::mutex dataMutex;
```

**activeRenderersAndLayers**  
The first set is the active renderers.  
The second set is the active layers.
```cpp
std::map <RendererMode, std::tuple<std::set<IRenderer*>, std::set<int>>> activeRenderersAndLayers;
```

### Functions:
**Private:**  
**AddData**  
```cpp
void AddData(const std::string& id, RendererMode rendererMode, RenderData* data, int layer);
```

**ChangeLayer**  
```cpp
void ChangeLayer(RenderData* data, int newLayer);
```

**RemoveData**  
```cpp
void RemoveData(RenderData* data);
```

**Public:**  
**RendererComponentHandler**  
```cpp
RendererComponentHandler();
```

**IsRendererExist**  
```cpp
bool IsRendererExist(const std::string& id, bool isThreadSafe);
```

**AddRenderer**  
```cpp
void AddRenderer(const std::string& id, IRenderer* renderer, bool isThreadSafe);
```

**RemoveRenderer**  
```cpp
void RemoveRenderer(const std::string& id, bool isThreadSafe);
```

**AddData**  
```cpp
void AddData(const std::string& id, RendererMode rendererMode, RenderData* data, int layer, bool isThreadSafe);
```

**ChangeLayer**  
```cpp
void ChangeLayer(RenderData* data, int newLayer, bool isThreadSafe);
```

**RemoveData**  
```cpp
void RemoveData(RenderData* data, bool isThreadSafe);
```

**SetDataToRenderers**  
It sets the data to the renderers, before the Run() function is called.
```cpp
void SetDataToRenderers();
```

**Run**  
It iterates through renderData, which use the rendererMode. Those data, which are
on the same layer and use the same id, they will be called by the same renderer.
The renderer will solve that how it can render them.  
It is called twice, once with RendererMode::RENDER and once with RendererMode::LATERENDER.
```cpp
void Run(RendererMode rendererMode);
```

**Clear**  
```cpp
void Clear();
```

##
## RendererMode
### Source Code:
[RendererMode.h](../../Learning2DEngine/Learning2DEngine/Render/RendererMode.h)  

### Description:
The modes of the RendererComponent.

### Header:
```cpp
enum class RendererMode
{
    RENDER,
    LATERENDER
};
```

##
## RenderManager
### Source Code:
[RenderManager.h](../../Learning2DEngine/Learning2DEngine/Render/RenderManager.h)  
[RenderManager.cpp](../../Learning2DEngine/Learning2DEngine/Render/RenderManager.cpp)  

### Description:
This is the heart of the rendering in the Engine.
The developer can initialize the OpenGL and set or change its variables.

### Header:
```cpp
class RenderManager : public System::Singleton<RenderManager>
{...}
```

### Variables:
**Private:**  
**window**  
```cpp
GLFWwindow* window;
```

**resolution**  
Note: the `Render::RenderManager`'s resolution and the `System::Camera`'s resolution are not same.  
The `Render::RenderManager`'s resolution is the real resolution, how the code render the game.  
The `System::Camera`'s resolution is like a coordinate system,
which the developer can use where they want to put the gameobjects.  
```cpp
Resolution resolution;
```

**clearColor**  
```cpp
glm::vec4 clearColor;
```

**keyboardEventHandler**  
```cpp
EventSystem::EventHandler<int, int, int, int> keyboardEventHandler;
```

**framebufferSizeEventHandler**  
```cpp
EventSystem::EventHandler<Resolution> framebufferSizeEventHandler;
```

**mouseButtonEventHandler**  
```cpp
EventSystem::EventHandler<int, int, int> mouseButtonEventHandler;
```

**cursorPositonEventHandler**  
```cpp
EventSystem::EventHandler<double, double> cursorPositonEventHandler;
```

**cursorEnterEventHandler**  
```cpp
EventSystem::EventHandler<bool> cursorEnterEventHandler;
```

**scrollEventHandler**  
```cpp
EventSystem::EventHandler<double, double> scrollEventHandler;
```

**blendFuncFactor**  
```cpp
BlendFuncFactor blendFuncFactor;
```

**isBlendActive**  
```cpp
bool isBlendActive;
```

**maxTextureUnits**  
```cpp
GLint maxTextureUnits;
```

**Public:**  
**mutex**  
It is a "global" mutex, that the OpenGL can be thread-safe.
```cpp
std::mutex mutex;
```

### Functions:
**Private:**  
**RenderManager**  
```cpp
RenderManager();
```

**CallbackUpdateKeyboard**  
It is used to subscribe to `glfwSetKeyCallback`.
```cpp
static void CallbackUpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
```

**UpdateKeyboard**  
`CallbackUpdateKeyboard` call this function.
```cpp
void UpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
```

**CallbackUpdateFramebufferSize**  
It is used to subscribe to `glfwSetFramebufferSizeCallback`.
```cpp
static void CallbackUpdateFramebufferSize(GLFWwindow* window, int width, int height);
```

**UpdateFramebufferSize**  
`CallbackUpdateFramebufferSize` call this function.
```cpp
void UpdateFramebufferSize(GLFWwindow* window, int width, int height);
```

**CallbackUpdateMouseButton**  
It is used to subscribe to `glfwSetMouseButtonCallback`.
```cpp
static void CallbackUpdateMouseButton(GLFWwindow* window, int button, int action, int mods);
```

**UpdateMouseButton**  
`CallbackUpdateMouseButton` call this function.
```cpp
void UpdateMouseButton(GLFWwindow* window, int button, int action, int mods);
```

**CallbackUpdateCursorPositon**  
It is used to subscribe to `glfwSetCursorPosCallback`.
```cpp
static void CallbackUpdateCursorPositon(GLFWwindow* window, double xpos, double ypos);
```

**UpdateCursorPositon**  
`CallbackUpdateCursorPositon` call this function.
```cpp
void UpdateCursorPositon(GLFWwindow* window, double xpos, double ypos);
```

**CallbackUpdateCursorEnter**  
It is used to subscribe to `glfwSetCursorEnterCallback`.
```cpp
static void CallbackUpdateCursorEnter(GLFWwindow* window, int entered);
```

**UpdateCursorEnter**  
`CallbackUpdateCursorEnter` call this function.
```cpp
void UpdateCursorEnter(GLFWwindow* window, int entered);
```

**CallbackUpdateScroll**  
It is used to subscribe to `glfwSetScrollCallback`.
```cpp
static void CallbackUpdateScroll(GLFWwindow* window, double xoffset, double yoffset);
```

**UpdateScroll**  
`CallbackUpdateScroll` call this function.
```cpp
void UpdateScroll(GLFWwindow* window, double xoffset, double yoffset);
```

**Public:**  
**Init**  
It initializes RenderManager.
```cpp
void Init(int majorVersion, int minorVersion, Resolution resolution, const char* title, bool resizableWindows = true);
```

**Terminate**  
It terminates the RenderManager.
```cpp
void Terminate();
```

**CloseWindow**  
It closes the (game) screen window.
```cpp
void CloseWindow();
```

**IsWindowClosed**  
It returns true, the the (game) screen window is closed.
```cpp
bool IsWindowClosed();
```

**SetVSync**  
It turns on/off the VSync.
```cpp
void SetVSync(bool value);
```

**UpdateWindow**  
It updates (game) screen window (swap the buffers).
```cpp
void UpdateWindow();
```

**ClearWindow**  
It clears the color buffer by the `clearColor` variable.
```cpp
void ClearWindow();
```

**GetResolution**  
It returns the (game) screen resolution.
```cpp
inline Resolution GetResolution() const;
```

**SetClearColor**  
It sets the `clearColor` for `ClearWindow()`;
```cpp
void SetClearColor(float r, float g, float b, float a);
```

**GetClearColor**  
It returns the current `clearColor`.
```cpp
inline glm::vec4 GetClearColor() const;
```

**EnableBlend**  
It enables the blend.
```cpp
void EnableBlend();
```

**DisableBlend**  
It disables the blend.
```cpp
void DisableBlend();
```

**IsBlendActive**  
It returns true if the blend is enabled.
```cpp
inline bool IsBlendActive() const;
```

**SetBlendFunc**  
It sets the BlendFunc.
```cpp
void SetBlendFunc(unsigned int sourceFactor, unsigned int destinationFactor);
```
```cpp
void SetBlendFunc(BlendFuncFactor blendFuncFactor);
```

**GetBlendFunc**  
It returns the current BlendFuncFactor.
```cpp
inline BlendFuncFactor GetBlendFunc() const;
```

**GetMaxTextureUnits**  
It returns how many texture units the GPU can handle at once.
```cpp
inline GLint GetMaxTextureUnits() const;
```

**AddKeyboardEvent**  
It adds event to keyboard events.
```cpp
void AddKeyboardEvent(EventSystem::EventItem<int, int, int, int>* eventItem);
```

**RemoveKeyboardEvent**  
It removes event from keyboard events.
```cpp
void RemoveKeyboardEvent(EventSystem::EventItem<int, int, int, int>* eventItem);
```

**AddFramebufferSizeEvent**  
It adds event to resolution changing events.
```cpp
void AddFramebufferSizeEvent(EventSystem::EventItem<Resolution>* eventItem);
```

**RemoveFramebufferSizeEvent**  
It removes event from resolution changing events.
```cpp
void RemoveFramebufferSizeEvent(EventSystem::EventItem<Resolution>* eventItem);
```

**AddMouseButtonEvent**  
It adds event to mouse button events.
```cpp
void AddMouseButtonEvent(EventSystem::EventItem<int, int, int>* eventItem);
```

**RemoveMouseButtonEvent**  
It removes event from mouse button events.
```cpp
void RemoveMouseButtonEvent(EventSystem::EventItem<int, int, int>* eventItem);
```

**AddCursorPositonEvent**  
It adds event to cursor position events.
```cpp
void AddCursorPositonEvent(EventSystem::EventItem<double, double>* eventItem);
```

**RemoveCursorPositonEvent**  
It removes event from cursor position events.
```cpp
void RemoveCursorPositonEvent(EventSystem::EventItem<double, double>* eventItem);
```

**AddCursorEnterEvent**  
It adds event to cursor enter events.
```cpp
void AddCursorEnterEvent(EventSystem::EventItem<bool>* eventItem);
```

**RemoveCursorEnterEvent**  
It removes event from cursor enter events.
```cpp
void RemoveCursorEnterEvent(EventSystem::EventItem<bool>* eventItem);
```

**AddScrollEvent**  
It adds event to scroll events.
```cpp
void AddScrollEvent(EventSystem::EventItem<double, double>* eventItem);
```

**RemoveScrollEvent**  
It removes event from scroll events.
```cpp
void RemoveScrollEvent(EventSystem::EventItem<double, double>* eventItem);
```

##
## Resolution
### Source Code:
[Resolution.h](../../Learning2DEngine/Learning2DEngine/Render/Resolution.h)

### Description:
The resolution of the screen/window
representation in the Engine.

### Header:
```cpp
class Resolution
{...}
```

### Variables:
**Private:**  
**width**  
```cpp
int width;
```

**height**  
```cpp
int height;
```

### Functions:
**Public:**  
**Resolution**  
```cpp
Resolution(int w, int h);
```

**GetWidth**  
```cpp
inline int GetWidth() const;
```

**GetHeight**  
```cpp
inline int GetHeight() const;
```

**ToString**  
```cpp
inline std::string ToString() const;
```

**ToVec2**  
```cpp
inline glm::vec2 ToVec2() const;
```

##
## Shader
### Source Code:
[Shader.h](../../Learning2DEngine/Learning2DEngine/Render/Shader.h)  
[Shader.cpp](../../Learning2DEngine/Learning2DEngine/Render/Shader.cpp)

### Description:
The OpenGL Shader representation in the Engine.

### Header:
```cpp
class Shader
{...}
```

### Auxiliary class
**Private:**  
```cpp
enum class ShaderType {VERTEX, FRAGMENT, GEOMETRY};
```

### Variables:
**Private:**  
**id**  
```cpp
GLuint id;
```

### Functions:
**Private:**  
**ToString**  
```cpp
static std::string ToString(ShaderType type);
```

**CheckShaderErrors**  
```cpp
void CheckShaderErrors(unsigned int shaderId, ShaderType type);
```

**CheckProgramErrors**  
```cpp
void CheckProgramErrors(unsigned int programId);
```

**Public:**  
**Shader**  
```cpp
Shader();
```

**Create**  
It creates the shader.
```cpp
void Create(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
```

**Destroy**  
It destroys the shader.
```cpp
void Destroy();
```

**Use**  
It binds the shader.
```cpp
void Use() const;
```

**GetId**  
It returns the id of shader.
```cpp
inline GLuint GetId();
```

**Functions to update the shader parameters** 
```cpp
void SetFloat(const char* name, float value);
void SetInteger(const char* name, int value);
void SetVector2f(const char* name, float x, float y);
void SetVector2f(const char* name, const glm::vec2& value);
void SetVector3f(const char* name, float x, float y, float z);
void SetVector3f(const char* name, const glm::vec3& value);
void SetVector4f(const char* name, float x, float y, float z, float w);
void SetVector4f(const char* name, const glm::vec4& value);
void SetMatrix4(const char* name, const glm::mat4& matrix);
void SetArray1f(const char* name, float* value, unsigned int length);
void SetArray1i(const char* name, int* value, unsigned int length);
void SetArray2f(const char* name, float* value, unsigned int length);
void SetArray2i(const char* name, int* value, unsigned int length);
```

##
## ShaderConstant
### Source Code:
[ShaderConstant.h](../../Learning2DEngine/Learning2DEngine/Render/ShaderConstant.h)  
[ShaderConstant.cpp](../../Learning2DEngine/Learning2DEngine/Render/ShaderConstant.cpp)

### Description:
The basic shaders of the Engine are in variables and not in files.

### Header:
```cpp
class ShaderConstant final
{...}
```

### Variables:
**Public:**  
**Simple Sprite shader**  
The simple sprite shader's name (for `ResourceManager`),
vertex and fragment shaders.
```cpp
static const std::string SIMPLE_SPRITE_SHADER_NAME;
static const char* GetSimpleSpriteVertexShader();
static const char* GetSimpleSpriteFragmentShader();
```

**Sprite shader**  
The sprite shader's name (for `ResourceManager`),
vertex and fragment shaders.  
It has multi instancing support.  
```cpp
static const std::string SPRITE_SHADER_NAME;
static const char* GetSpriteVertexShader();
static const char* GetSpriteFragmentShader();
```

**Simple Text2D shader**  
The simple Text2D shader's name (for `ResourceManager`),
vertex and fragment shaders.
```cpp
static const std::string SIMPLE_TEXT2D_SHADER_NAME;
static const char* GetSimpleText2DVertexShader();
static const char* GetSimpleText2DFragmentShader();
```

**Text2D shader**  
The Text2D shader's name (for `ResourceManager`),
vertex and fragment shaders.
It has multi instancing support.  
```cpp
static const std::string TEXT2D_SHADER_NAME;
static const char* GetText2DVertexShader();
static const char* GetText2DFragmentShader();
```

**Default PostProcessEffect shader**  
The default PostProcessEffect shader's name (for `ResourceManager`),
vertex and fragment shaders.
```cpp
static const std::string DEFAULT_POSTPROCESS_EFFECT_NAME;
static const char* GetDefaultPostprocessVertexShader();
static const char* GetDefaultPostprocessFragmentShader();
```

**Base Color shader**  
The base color shader's name (for `ResourceManager`),
vertex and fragment shaders.
```cpp
static const std::string BASE_COLOR_NAME;
static const char* GetBaseColorVertexShader();
static const char* GetBaseColorFragmentShader();
```

##
## SimpleSpriteRenderComponent
### Source Code:
[SimpleSpriteRenderComponent.h](../../Learning2DEngine/Learning2DEngine/Render/SimpleSpriteRenderComponent.h)  
[SimpleSpriteRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/Render/SimpleSpriteRenderComponent.cpp)

### Description:
It is for render a sprite with color and texture.  
It uses `SimpleSpriteRenderer` for rendering. It is recommand, when the developer knows,
the object has just a couple instances in a layer.  
Please more info about `RendererComponent`.

### Header:
```cpp
class SimpleSpriteRenderComponent : public RendererComponent<SpriteRenderData, SimpleSpriteRenderer>
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
**SimpleSpriteRenderComponent**  
```cpp
SimpleSpriteRenderComponent(System::GameObject* gameObject, RendererMode mode, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
SimpleSpriteRenderComponent(System::GameObject* gameObject, RendererMode mode, const Texture2D& texture, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetInitedRenderer**  
```cpp
SimpleSpriteRenderer* GetInitedRenderer() override;
```

**DestroyRenderer**  
```cpp
void DestroyRenderer() override;
```

##
## SimpleSpriteRenderer
### Source Code:
[SimpleSpriteRenderer.h](../../Learning2DEngine/Learning2DEngine/Render/SimpleSpriteRenderer.h)  
[SimpleSpriteRenderer.cpp](../../Learning2DEngine/Learning2DEngine/Render/SimpleSpriteRenderer.cpp)

### Description:
It can render the sprites. 
It has better performance, than the `MultiSpriteRenderer`,
when the object has just a couple instances in a layer.  
Note: The projection and the view matrix came from Game::mainCamera.

### Header:
```cpp
class SimpleSpriteRenderer : public BaseRenderer, public System::Singleton<SimpleSpriteRenderer>
{...}
```

### Variables:
**Private:**  
**spriteRenderData**  
Note: The int is the layer.  
```cpp
std::map<RendererMode, std::map<int, std::vector<RenderData*>>> spriteRenderData;
```

### Functions:
**Private:**  
**SimpleSpriteRenderer**  
```cpp
SimpleSpriteRenderer();
```

**InitShader**  
```cpp
void InitShader() override;
```

**InitVao**  
```cpp
void InitVao() override;
```

**DestroyObject**  
The Destroy() call it with or without mutex.
```cpp
void DestroyObject() override;
```

**Public:**  
**SetData**  
Note: the int is the layer.
```cpp
void SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData) override;
```

**Draw**  
It draws those objects, which was added with SetData and they are on the selected mode and layer.
```cpp
void Draw(RendererMode rendererMode, int layer) override;
```


##
## SpriteRenderComponent
### Source Code:
[SpriteRenderComponent.h](../../Learning2DEngine/Learning2DEngine/Render/SpriteRenderComponent.h)  
[SpriteRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/Render/SpriteRenderComponent.cpp)

### Description:
It is for render a sprite with color and texture.   
It uses `MultiSpriteRenderer` for rendering. It is recommand, when the developer knows,
the object has a lot of instances in a layer.   
It supports the multi instance rendering.  
Please more info about `RendererComponent`.

### Header:
```cpp
class SpriteRenderComponent : public RendererComponent<SpriteRenderData, MultiSpriteRenderer>
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
**SpriteRenderComponent**  
```cpp
SpriteRenderComponent(System::GameObject* gameObject, RendererMode mode,int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
SpriteRenderComponent(System::GameObject* gameObject, RendererMode mode,const Texture2D& texture, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetInitedRenderer**  
```cpp
MultiSpriteRenderer* GetInitedRenderer() override;
```

**DestroyRenderer**  
```cpp
void DestroyRenderer() override;
```

##
## SpriteRenderData
### Source Code:
[SpriteRenderData.h](../../Learning2DEngine/Learning2DEngine/Render/SpriteRenderData.h)  

### Description:
It contains the data, which is important to render the sprite.

### Header:
```cpp
class SpriteRenderData : public RenderData, public Texture2DContainer
{...}
```

### Variables:
**Public:**  
**color**
```cpp
glm::vec4 color;
```

**isUseCameraView**  
It shows, that the sprite should be rendered with camera view matrix or not.  
Note: default value is true.
```cpp
bool isUseCameraView;
```

### Functions:
**Public:**  
**SpriteRenderData**
```cpp
SpriteRenderData(const System::Component* component, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
SpriteRenderData(const System::Component* component, bool isUseCameraView, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
SpriteRenderData(const System::Component* component, const Texture2D& texture, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
SpriteRenderData(const System::Component* component, const Texture2D& texture, bool isUseCameraView, glm::vec4 color = glm::vec4(1.0f));
```

##
## Texture2D
### Source Code:
[Texture2D.h](../../Learning2DEngine/Learning2DEngine/Render/Texture2D.h)  
[Texture2D.cpp](../../Learning2DEngine/Learning2DEngine/Render/Texture2D.cpp)

### Description:
The OpenGL Texture2D representation in the Engine.  

### Header:
```cpp
class Texture2D
{...}
```

### Variables:
**Private:**  
**id**  
```cpp
GLuint id;
```  

**width**  
Width of the loaded image in pixels.
```cpp
unsigned int width;
``` 

**height**  
Height of the loaded image in pixels
```cpp
unsigned int height;
``` 

**settings**  
```cpp
Texture2DSettings settings;
``` 

### Functions:
**Public:**  
**Texture2D**  
```cpp
Texture2D();
```
```cpp
Texture2D(const Texture2DSettings& settings);
```

**Create**  
It creates the `Texture2D`.
```cpp
void Create(unsigned int width, unsigned int height, const unsigned char* data);
```

**Destroy**  
It destroys the `Texture2D`.
```cpp
void Destroy();
```

**Bind**  
It binds the `Texture2D` and it activates the texture with glActiveTexture.
```cpp
void Bind(int textureUnitNumber = 0) const;
```

**GetId**  
It returns the id of the `Texture2D`.
```cpp
inline GLuint GetId() const;
```

**GetWidth**  
```cpp
inline unsigned int GetWidth() const;
```

**GetHeight**  
```cpp
inline unsigned int GetHeight() const;
```

**Texture2DSettings**  
```cpp
inline const Texture2DSettings& GetSettings() const;
```

##
## Texture2DContainer
### Source Code:
[Texture2DContainer.h](../../Learning2DEngine/Learning2DEngine/Render/Texture2DContainer.h) 

### Description:
It is a simple class, which can contain an texture.  
If a class is inherited from this, it will has a texture with some useful functions.  
Moreover, the `AnimationController` contains a reference from this for working.

### Header:
```cpp
class Texture2DContainer
{...}
```

### Variables:
**Private:**  
**texture**  
```cpp
const Texture2D* texture;
``` 

### Functions:
**Public:**  
**Texture2DContainer**  
It will not copy the texture, it will use texture reference.  
```cpp
Texture2DContainer();
```
```cpp
Texture2DContainer(const Texture2D& texture);
```

**~Texture2DContainer**  
```cpp
virtual ~Texture2DContainer() = default;
```

**SetTexture**  
It will not copy the texture, it will use texture reference.  
```cpp
void SetTexture(const Texture2D& texture);
```

**GetTexture**  
```cpp
inline const Texture2D* GetTexture() const;
```

**ClearTexture**  
```cpp
inline void ClearTexture();
```

**IsUseTexture**  
```cpp
inline bool IsUseTexture() const;
```

##
## Texture2DSettings
### Source Code:
[Texture2DSettings.h](../../Learning2DEngine/Learning2DEngine/Render/Texture2DSettings.h)  

### Description:
It is a collection of setting parameters for `Texture2D`.
It uses OpenGL constants like *GL_RGB*, *GL_REPEAT*, *GL_LINEAR* etc. 

### Header:
```cpp
struct Texture2DSettings
{
    // Format of the texture object
    unsigned int internalFormat;
    // Format of the loaded image
    unsigned int imageFormat;
    // Wrapping mode on S axis
    unsigned int wrapS;
    // Wrapping mode on T axis
    unsigned int wrapT;
    // Filtering mode if texture pixels < window pixels
    unsigned int filterMin;
    // Filtering mode if texture pixels > window pixels
    unsigned int filterMax;

    Texture2DSettings()
        : internalFormat(GL_RGB), imageFormat(GL_RGB),
            wrapS(GL_REPEAT), wrapT(GL_REPEAT),
            filterMin(GL_LINEAR), filterMax(GL_LINEAR)
    {

    }
};
```

            