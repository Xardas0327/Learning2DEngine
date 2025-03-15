# UI
- [FontSizePair](UI.md#fontsizepair)
- [FreeTypeCharacter](UI.md#freetypecharacter)
- [Text2DLateRenderComponent](UI.md#text2dlaterendercomponent)
- [Text2DRenderData](UI.md#text2drenderdata)
- [Text2DRenderer](UI.md#text2drenderer)
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
    unsigned int advance;
};
```

##
## Text2DLateRenderComponent
### Source Code:
[Text2DLateRenderComponent.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DLateRenderComponent.h)  
[Text2DLateRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/UI/Text2DLateRenderComponent.cpp)

### Description:
It can render a text with a color.
It uses static variables to count how many GameObject initialized it.
That's why it will destroy renderer only
if the reference number is 0, otherway it will decrease
the reference number.  
Please more info about `LateRendererComponent`.

### Header:
```cpp
class Text2DLateRenderComponent : public virtual Render::LateRendererComponent<Text2DRenderData, Text2DRenderer>
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

**Protected:**  
**mutex**  
```cpp
std::mutex mutex;
```

### Functions:
**Protected:**  
**Text2DLateRenderComponent**  
```cpp
Text2DLateRenderComponent(System::GameObject* gameObject, const FontSizePair& fontSizePair, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
Text2DLateRenderComponent(System::GameObject* gameObject, const FontSizePair& fontSizePair, std::string text, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
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
Text2DRenderer* GetRenderer() const override;
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
Text2DRenderData(const System::Component* component, const glm::vec4 color = glm::vec4(1.0f));
```
```cpp
Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, const std::string& text, glm::vec4 color = glm::vec4(1.0f));
```

**GetRotationMatrix**
```cpp
glm::mat2 GetRotationMatrix();
```

##
## Text2DRenderer
### Source Code:
[Text2DRenderer.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DRenderer.h)  
[Text2DRenderer.cpp](../../Learning2DEngine/Learning2DEngine/UI/Text2DRenderer.cpp)

### Description:
It can render texts.
Note: The projection came from Game::mainCamera.

### Header:
```cpp
class Text2DRenderer : public Render::IRenderer, public virtual System::Singleton<Text2DRenderer>
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

### Functions:
**Private:**  
**SpriteRenderer**  
```cpp
SpriteRenderer();
```

**InitShader**  
```cpp
void InitShader();
```

**InitVao**  
```cpp
void InitVao();
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

**Draw**  
```cpp
void Draw(std::vector<Render::RenderData*> renderData) override;
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
class TextCharacterSet : public virtual System::Singleton<TextCharacterSet>
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
