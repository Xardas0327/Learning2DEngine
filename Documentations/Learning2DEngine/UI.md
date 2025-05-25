# UI
- [FontSizePair](UI.md#fontsizepair)
- [FreeTypeCharacter](UI.md#freetypecharacter)
- [MultiText2DRenderer](UI.md#multitext2drenderer)
- [SimpleText2DLateRenderComponent](UI.md#simpletext2dlaterendercomponent)
- [SimpleText2DRenderer](UI.md#simpletext2drenderer)
- [Text2DLateRenderComponent](UI.md#text2dlaterendercomponent)
- [Text2DDynamicData](UI.md#text2ddynamicdata)
- [Text2DRenderData](UI.md#text2drenderdata)
- [TextCharacterSet](UI.md#textcharacterset)

##
## FontSizePair
### Source Code:
[FontSizePair.h](../../Learning2DEngine/Learning2DEngine/UI/FontSizePair.h).

### Description:
It is a simple `std::pair<std::string, unsigned int>`.  
The first variable is the font's name (path).  
The second variable is the size of the font.

### Header:
```cpp
typedef std::pair<std::string, unsigned int> FontSizePair;
```

##
## FreeTypeCharacter
### Source Code:
[FreeTypeCharacter.h](../../Learning2DEngine/Learning2DEngine/UI/FreeTypeCharacter.h)

### Description:
This is the FreeType characters' representation in the Engine.
It is used by the `TextCharacterSet`.
If you want to understand the bearing and advance parameters more,
please check their documentation [here](https://freetype.org/freetype2/docs/glyphs/glyphs-3.html#section-3).

### Header:
```cpp
struct FreeTypeCharacter
{
    unsigned int textureId;
    glm::ivec2   size;
    glm::ivec2   bearing;
    FT_Pos advance;
};
```


##
## MultiText2DRenderer
### Source Code:
[MultiText2DRenderer.h](../../Learning2DEngine/Learning2DEngine/UI/MultiText2DRenderer.h)  
[MultiText2DRenderer.cpp](../../Learning2DEngine/Learning2DEngine/UI/MultiText2DRenderer.cpp)

### Description:
It can render texts.
A Text2D renderer, which has multi instancing support.  
It has better performance, than the `SimpleText2DRenderer`,
when there are huge texts.  
Note: The projection came from Game::mainCamera.

### Header:
```cpp
class MultiText2DRenderer : public Render::IRenderer, public System::Singleton<MultiText2DRenderer>
{...}
```

### Variables:
**Private:**  
**shader**  
```cpp
Render::Shader shader;
```

**vao**  
```cpp
GLuint vao;
```

**ebo**  
```cpp
GLuint ebo;
```

**vboDynamic**  
It contains the uploaded data of the renderable characters.  
Its size is depend on the maxObjectSize.
```cpp
GLuint vboDynamic;
```
**maxObjectSize**  
```cpp
size_t maxObjectSize;
```

**textRenderData**  
The first int is the layer. 
The map key is a GLuint, which a character texture id, and the value is a vector of array,
which contains the position, the texture coordinates and the color..
```cpp
std::map<int, std::map<GLuint, std::vector<std::array<float, 32>>>> textRenderData;
```

**dynamicData**  
It is array, which contains the characters positions and colors of the renderable objects, before the upload.
Note: its size is depend on the maxObjectSize, so it will be reallocated only, when the maxObjectSize is changed.
```cpp
Text2DDynamicData* dynamicData;
```

### Functions:
**Private:**  
**MultiText2DRenderer**  
```cpp
MultiText2DRenderer();
```

**InitShader**  
```cpp
void InitShader();
```

**InitVao**  
```cpp
void InitVao();
```

**DestroyObject**  
The Destroy() call it with or without mutex.
```cpp
void DestroyObject();
```

**Public:**  
**Init**  
```cpp
void Init() override;
```

**Destroy**  
```cpp
void Destroy() override;
```

**SetData**  
Note: the int is the layer.
```cpp
void SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData) override;
```

**Draw**  
It draws those objects, which was added with SetData and they are on the selected layer.
```cpp
void Draw(int layer) override;
```


##
## SimpleText2DLateRenderComponent
### Source Code:
[SimpleText2DLateRenderComponent.h](../../Learning2DEngine/Learning2DEngine/UI/SimpleText2DLateRenderComponent.h)  
[SimpleText2DLateRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/UI/SimpleText2DLateRenderComponent.cpp)

### Description:
It can render a text with a color.  
It uses `SimpleText2DRenderer` for rendering. It is recommand, when the developer knows,
there are only little texts.  
It uses static variables to count how many GameObject initialized it.
That's why it will destroy renderer only
if the reference number is 0, otherway it will decrease
the reference number.  
Please more info about `LateRendererComponent`.

### Header:
```cpp
class SimpleText2DLateRenderComponent : public Render::LateRendererComponent<Text2DRenderData, SimpleText2DRenderer>
{...}
```

### Variables:
**Private:**  
**id**  
```cpp
static const std::string id;
```

**refrenceNumber**  
It is counted, that how many SpriteRenderComponent exist.
```cpp
static int refrenceNumber;
```

**mutex**  
```cpp
static std::mutex mutex;
```

### Functions:
**Protected:**  
**SimpleText2DLateRenderComponent**  
```cpp
SimpleText2DLateRenderComponent(System::GameObject* gameObject, const FontSizePair& fontSizePair, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
SimpleText2DLateRenderComponent(System::GameObject* gameObject, const FontSizePair& fontSizePair, const std::string& text, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```

**Init**  
```cpp
void Init() override;
```

**Destroy**  
```cpp
void Destroy() override;
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetRenderer**  
```cpp
SimpleText2DRenderer* GetRenderer() const override;
```


##
## SimpleText2DRenderer
### Source Code:
[SimpleText2DRenderer.h](../../Learning2DEngine/Learning2DEngine/UI/SimpleText2DRenderer.h)  
[SimpleText2DRenderer.cpp](../../Learning2DEngine/Learning2DEngine/UI/SimpleText2DRenderer.cpp)

### Description:
It can render texts.  
It has better performance, than the `MultiText2DRenderer`,
when there are only little texts.  
Note: The projection came from Game::mainCamera.

### Header:
```cpp
class SimpleText2DRenderer : public Render::IRenderer, public System::Singleton<SimpleText2DRenderer>
{...}
```

### Variables:
**Private:**  
**shader**  
```cpp
Render::Shader shader;
```

**vao**  
```cpp
GLuint vao;
```

**vbo**  
```cpp
GLuint vbo;
```

**ebo**  
```cpp
GLuint ebo;
```

**textRenderData**  
Note: the int is the layer.
```cpp
std::map<int, std::vector<Render::RenderData*>> textRenderData;
```

### Functions:
**Private:**  
**SimpleText2DRenderer**  
```cpp
SimpleText2DRenderer();
```

**InitShader**  
```cpp
void InitShader();
```

**InitVao**  
```cpp
void InitVao();
```

**DestroyObject**  
The Destroy() call it with or without mutex.
```cpp
void DestroyObject();
```

**Public:**  
**Init**  
```cpp
void Init() override;
```

**Destroy**  
```cpp
void Destroy() override;
```

**SetData**  
Note: the int is the layer.
```cpp
void SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData) override;
```

**Draw**  
It draws those objects, which was added with SetData and they are on the selected layer.
```cpp
void Draw(int layer) override;
```


##
## Text2DLateRenderComponent
### Source Code:
[Text2DLateRenderComponent.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DLateRenderComponent.h)  
[Text2DLateRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/UI/Text2DLateRenderComponent.cpp)

### Description:
It can render a text with a color.  
It uses `MultiText2DRenderer` for rendering. It is recommand, when the developer knows,
there are huge texts.  
It uses static variables to count how many GameObject initialized it.
That's why it will destroy renderer only
if the reference number is 0, otherway it will decrease
the reference number.  
Please more info about `LateRendererComponent`.

### Header:
```cpp
class Text2DLateRenderComponent : public Render::LateRendererComponent<Text2DRenderData, MultiText2DRenderer>
{...}
```

### Variables:
**Private:**  
**id**  
```cpp
static const std::string id;
```

**refrenceNumber**  
It is counted, that how many SpriteRenderComponent exist.
```cpp
static int refrenceNumber;
```

**mutex**  
```cpp
static std::mutex mutex;
```

### Functions:
**Protected:**  
**Text2DLateRenderComponent**  
```cpp
Text2DLateRenderComponent(System::GameObject* gameObject, const FontSizePair& fontSizePair, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
Text2DLateRenderComponent(System::GameObject* gameObject, const FontSizePair& fontSizePair, const std::string& text, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```

**Init**  
```cpp
void Init() override;
```

**Destroy**  
```cpp
void Destroy() override;
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetRenderer**  
```cpp
MultiText2DRenderer* GetRenderer() const override;
```

##
## Text2DDynamicData
### Source Code:
[Text2DDynamicData.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DDynamicData.h)   

### Description:
It contains the dynamic data of the `MultiText2DRenderer`.  
Note: the textureId is float, because we sent it to vertex shader, but it will be converted to int.

### Header:
```cpp
struct Text2DDynamicData
{
	float position[2];
	float textCoord[2];
	float color[4];
	float textureId;
};
```

##
## Text2DRenderData
### Source Code:
[Text2DRenderData.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DRenderData.h)  

### Description:
It contains the data, which is important to render a text.

### Header:
```cpp
struct Text2DRenderData : public Render::RenderData
{...}
```

### Variables:
**Public:**  
**fontSizePair**
```cpp
FontSizePair fontSizePair;
```

**text**
```cpp
std::string text;
```

**color**
```cpp
glm::vec4 color;
```

### Functions:
**Public:**  
**Text2DRenderData**
```cpp
Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, const std::string& text, glm::vec4 color = glm::vec4(1.0f));
```

**GetRotationMatrix**
```cpp
glm::mat2 GetRotationMatrix();
```

##
## TextCharacterSet
### Source Code:
[TextCharacterSet.h](../../Learning2DEngine/Learning2DEngine/UI/TextCharacterSet.h)  
[TextCharacterSet.cpp](../../Learning2DEngine/Learning2DEngine/UI/TextCharacterSet.cpp)

### Description:
This is a singleton class, which can load/unload the fonts into Engine.

### Header:
```cpp
class TextCharacterSet : public System::Singleton<TextCharacterSet>
{...}
```

### Macros:
**L2DE_FONT_NUMBER**  
The `TextCharacterSet` load the first `L2DE_FONT_NUMBER` characters from a ttf file.
Its value is 128.

### Auxiliary classes:
**CharacterMap**  
```cpp
typedef std::map<char, FreeTypeCharacter> CharacterMap;
```

### Variables:
**Private:**  
**characters**  
```cpp
std::map<FontSizePair, CharacterMap> characters;
```

### Functions:
**Private:**  
**TextCharacterSet**  
```cpp
TextCharacterSet();
```

**Public:**  
**operator[]**  
The non-const version try to load the font with the size, if it is not loaded yet.
```cpp
CharacterMap& operator[](const FontSizePair& fontSizePair);
```  
```cpp
const CharacterMap& operator[](const FontSizePair& fontSizePair) const;
```  

**Load**  
They load the first `L2DE_FONT_NUMBER` characters from a ttf file with a size.
```cpp
void Load(const std::string& font, unsigned int fontSize);
```
```cpp
void Load(const FontSizePair& fontSizePair);
```

**Unload**  
They unload a font with a size.
```cpp
void Unload(const std::string& font, unsigned int fontSize);
```
```cpp
void Unload(const FontSizePair& fontSizePair);
```

**Clear**  
It unloads all font with all size.
```cpp
void Clear();
```
