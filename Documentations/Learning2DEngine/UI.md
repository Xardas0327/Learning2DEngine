# UI
- [FontSizePair](UI.md#fontsizepair)
- [FreeTypeCharacter](UI.md#freetypecharacter)
- [Text2DLateRenderer](UI.md#text2dlaterenderer)
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
## Text2DLateRenderer
### Source Code:
[Text2DLateRenderer.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DLateRenderer.h)  
[Text2DLateRenderer.cpp](../../Learning2DEngine/Learning2DEngine/UI/Text2DLateRenderer.cpp)

### Description:
The `Text2DLateRenderer` is for the text rendering to UI.  
It uses static variables to count how many GameObject initialized it.
That's why it will destroy its Vertex Array Object only
if the reference number is 0, otherway it will decrease
the reference number only.  
Please more info about `LateRendererComponent`.

### Header:
```cpp
class Text2DLateRenderer : public virtual Render::LateRendererComponent
{...}
```

### Variables:
**Private:**  
**referenceNumber**  
All `Text2DLateRenderer` use the same shader and vertex array object.
That's why it is counted, that how many `Text2DLateRenderer` there are in the game.
It is important, that the shader will be created if it is used and
it will be destroyed if nothing uses it.  
Note: The projection is came from Game::mainCamera.
```cpp
static int referenceNumber;
```

**shader**  
```cpp
static Shader shader
```

**vao**  
```cpp
static GLuint vao;
```

**vbo**  
```cpp
static GLuint vbo;
```

**ebo**  
```cpp
static GLuint ebo;
```

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
**Text2DLateRenderer**  
```cpp
Text2DLateRenderer(System::GameObject* gameObject, const FontSizePair& fontSizePair, int layer = 0, glm::vec3 color = glm::vec3(1.0f));
```
```cpp
Text2DLateRenderer(System::GameObject* gameObject, const FontSizePair& fontSizePair, std::string text, int layer = 0, glm::vec3 color = glm::vec3(1.0f));
```

**Init**  
It initializes the `Text2DLateRenderer`.
```cpp
void Init() override;
```

**Destroy**  
It destroys the `Text2DLateRenderer`.
```cpp
void Destroy() override;
```

**Draw**  
It draws the text. 
```cpp
void Draw() override;
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
