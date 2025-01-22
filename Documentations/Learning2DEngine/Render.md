# Render
- [BlendFuncFactor](Render.md#blendfuncfactor)
- [IResolutionRefresher](Render.md#iresolutionrefresher)
- [MSAA](Render.md#msaa)
- [PostProcessEffect](Render.md#postprocesseffect)
- [RenderManager](Render.md#rendermanager)
- [Resolution](Render.md#resolution)
- [Shader](Render.md#shader)
- [ShaderConstant](Render.md#shaderconstant)
- [SpriteRenderer](Render.md#spriterenderer)
- [Texture2D](Render.md#texture2d)
- [Texture2DSettings](Render.md#texture2dsettings)

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
~MSAA();
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
Shader defaultShader;
```

**effectShader**  
```cpp
Shader effectShader;
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

**isEffectUsed**  
```cpp
bool isEffectUsed;
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
## RenderManager
### Source Code:
[RenderManager.h](../../Learning2DEngine/Learning2DEngine/Render/RenderManager.h)  
[RenderManager.cpp](../../Learning2DEngine/Learning2DEngine/Render/RenderManager.cpp)  

### Description:
This is the heart of the rendering in the Engine.
The developer can initialize the OpenGL and set or change its variables.

### Header:
```cpp
class RenderManager : public virtual System::Singleton<RenderManager>
{...}
```

### Variables:
**Private:**  
**window**  
```cpp
GLFWwindow* window;
```

**resolution**  
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

**isBlendActive**  
```cpp
bool isBlendActive;
```

**blendFuncFactor**  
```cpp
BlendFuncFactor blendFuncFactor;
```

### Functions:
**Private:**  
**RenderManager**  
```cpp
RenderManager();
```

**CallbackUpdateKeyboardMouse**  
It is used to subscribe to `glfwSetKeyCallback`.
```cpp
static void CallbackUpdateKeyboardMouse(GLFWwindow* window, int key, int scancode, int action, int mode);
```

**UpdateKeyboardMouse**  
`CallbackUpdateKeyboardMouse` call this function.
```cpp
void UpdateKeyboardMouse(GLFWwindow* window, int key, int scancode, int action, int mode);
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
inline Resolution GetResolution();
```

**SetClearColor**  
It sets the `clearColor` for `ClearWindow()`;
```cpp
void SetClearColor(float r, float g, float b, float a);
```

**GetClearColor**  
It returns the current `clearColor`.
```cpp
inline glm::vec4 GetClearColor();
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
inline bool IsBlendActive();
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
inline BlendFuncFactor GetBlendFunc();
```

**AddKeyboardEvent**  
It adds event to Keyboard and Mouse events.
```cpp
void AddKeyboardEvent(EventSystem::EventItem<int, int, int, int>* evenItem);
```

**RemoveKeyboardEvent**  
It removes event from Keyboard and Mouse events.
```cpp
void RemoveKeyboardEvent(EventSystem::EventItem<int, int, int, int>* evenItem);
```

**AddFramebufferSizeEvent**  
It adds event to Resolution changing events.
```cpp
void AddFramebufferSizeEvent(EventSystem::EventItem<Resolution>* evenItem);
```

**RemoveFramebufferSizeEvent**  
It removes event from Resolution changing events.
```cpp
void RemoveFramebufferSizeEvent(EventSystem::EventItem<Resolution>* evenItem);
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
enum ShaderType {VERTEX, FRAGMENT, GEOMETRY};
```

### Variables:
**Private:**  
**id**  
```cpp
unsigned int id;
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
inline unsigned int GetId();
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
**Sprite shader**  
The sprite shader's name (for `ResourceManager`),
vertex and fragment shaders.
```cpp
static const std::string SPRITE_SHADER_NAME;
static const char* const SPRITE_VERTEX_SHADER;
static const char* const SPRITE_FRAGMENT_SHADER;
```

**Text2D shader**  
The Text2D shader's name (for `ResourceManager`),
vertex and fragment shaders.
```cpp
static const std::string TEXT2D_SHADER_NAME;
static const char* const TEXT2D_VERTEX_SHADER;
static const char* const TEXT2D_FRAGMENT_SHADER;
```

**Default PostProcessEffect shader**  
The default PostProcessEffect shader's name (for `ResourceManager`),
vertex and fragment shaders.
```cpp
static const std::string DEFAULT_POSTPROCESS_EFFECT_NAME;
static const char* const DEFAULT_POSTPROCESS_EFFECT_VERTEX_SHADER;
static const char* const DEFAULT_POSTPROCESS_EFFECT_FRAGMENT_SHADER;
```

##
## SpriteRenderer
### Source Code:
[SpriteRenderer.h](../../Learning2DEngine/Learning2DEngine/Render/SpriteRenderer.h)  
[SpriteRenderer.cpp](../../Learning2DEngine/Learning2DEngine/Render/SpriteRenderer.cpp)

### Description:
The `SpriteRenderer` is for the `GameObjects`.
It uses static variables to count how many GameObject initialized it.
That's why it will destroy its Vertex Array Object only
if the reference number is 0, otherway it will decrease
the reference number only.  
Please more info about `RendererComponent`.

### Header:
```cpp
class SpriteRenderer : public virtual RendererComponent
{...}
```

### Variables:
**Private:**  
**isInit**  
```cpp
bool isInit;
```

**referenceNumber**  
All `SpriteRenderer` use the same shader and vertex array object.
That's why it is counted, that how many `SpriteRenderers` there are in the game.
It is important, that the shader will be created if it is used and
it will be destroyed if nothing uses it.
```cpp
static int referenceNumber;
```

**shader**  
```cpp
static Shader shader
```

**vao**  
```cpp
static unsigned int vao;
```

**vbo**  
```cpp
static unsigned int vbo;
```

**ebo**  
```cpp
static unsigned int ebo;
```

**Public:**  
**texture**  
The `texture`, which the `SpriteRenderer` will use.
This can be useful if the developer want to
change anything in the `texture`.
But if the developer want to remove it,
they should use the `ClearTexture()` function.
```cpp
Texture2D* texture;
```

**color**  
The `color`, which will be mixed with the texture in the shader.
```cpp
glm::vec3 color;
```

### Functions:
**Private:**  
**InitShader**  
```cpp
void InitShader();
```

**InitVao**  
```cpp
void InitVao();
```

**Protected:**  
**SpriteRenderer**  
```cpp
SpriteRenderer(System::GameObject* gameObject, int layer = 0, glm::vec3 color = glm::vec3(1.0f));
```
```cpp
SpriteRenderer(System::GameObject* gameObject, const Texture2D& texture, int layer = 0, glm::vec3 color = glm::vec3(1.0f));
```

**Init**  
It initializes the `SpriteRenderer`.
```cpp
void Init() override;
```

**Destroy**  
It destroys the `SpriteRenderer`.
```cpp
void Destroy() override;
```

**Public:**  
**~SpriteRenderer**  
It will delete the `texture` pointer.  
It won't call the `Destroy()` of the `texture`.
That's why the texture still will be available in the `ResourceManager`. 
If the `texture` was created by the developer,
they have to care with this problem.
```cpp
~SpriteRenderer();
``` 

**Draw**  
It draws the sprite. 
```cpp
void Draw() override;
``` 

**Draw**  
It draws the sprite. 
```cpp
void Draw() override;
``` 

**IsUseTexture**  
It returns true, if `SpriteRenderer` has `texture`.
```cpp
inline bool IsUseTexture();
``` 

**ClearTexture**  
It deletes the texture pointer.
But it won't call the `Destroy()` of the `texture`.
```cpp
inline bool ClearTexture();
``` 

##
## Texture2D
### Source Code:
[Texture2D.h](../../Learning2DEngine/Learning2DEngine/Render/Texture2D.h)  
[Texture2D.cpp](../../Learning2DEngine/Learning2DEngine/Render/Texture2D.cpp)

### Description:
The OpenGL Texture2D representation in the Engine.
If the developer changes any variables in it,
it will be used only after `Create` function.

### Header:
```cpp
class Texture2D
{...}
```

### Variables:
**Private:**  
**id**  
```cpp
unsigned int id;
``` 

**Public:**  
**width**  
```cpp
unsigned int width;
``` 

**height**  
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
It binds the `Texture2D`.
```cpp
void Bind() const;
```

**GetId**  
It returns the id of the `Texture2D`.
```cpp
inline unsigned int GetId();
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

            