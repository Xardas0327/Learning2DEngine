# UI
- [FontSizePair](UI.md#fontsizepair)
- [FreeTypeCharacter](UI.md#freetypecharacter)
- [Text](UI.md#text)
- [Text2DRenderer](UI.md#text2drenderer)

##
## FontSizePair
### Source Code:
It does not have separate source file. It is in
[Text.h](../../Learning2DEngine/Learning2DEngine/EventSystem/EventHandler.h).

### Description:
It is a simple `std::pair<std::string, unsigned int>`.  
The first variable is the font name (path).  
The second variable is the size of the font.  
`Text2DRenderer` can load the fonts by this pair.

### Header:
```cpp
typedef std::pair<std::string, unsigned int> FontSizePair;
```

##
## FreeTypeCharacter
### Source Code:
[FreeTypeCharacter.h](../../Learning2DEngine/Learning2DEngine/EventSystem/FreeTypeCharacter.h)

### Description:
This is the FreeType characters' representation in the Engine.
It is used by the `Text2DRenderer`.
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
## Text
### Source Code:
[Text.h](../../Learning2DEngine/Learning2DEngine/EventSystem/Text.h)

### Description:
This is the texts' representation in the Engine.
The Text has position, color and scale too.
The scale can be useful if the developer want load lesser fonts.

### Header:
```cpp
struct Text
{
    FontSizePair fontSizePair;
    std::string text;
    float x = 0;
    float y = 0;
    float scale = 1.0f;
    glm::vec3 color = glm::vec3(1.0f);
};
```

##
## Text2DRenderer
### Source Code:
[Text2DRenderer.h](../../Learning2DEngine/Learning2DEngine/EventSystem/Text2DRenderer.h)  
[Text2DRenderer.cpp](../../Learning2DEngine/Learning2DEngine/EventSystem/Text2DRenderer.cpp)

### Description:
This is a singleton class, which can load/unload the fonts into Engine
and it can render the Texts.

### Header:
```cpp
class Text2DRenderer : public virtual System::Singleton<Text2DRenderer>
{...}
```

### Macros:
**FONT_NUMBER**  
The `Text2DRenderer` load the first `FONT_NUMBER` characters from a ttf file.

### Variables:
**Private:**  
**CharacterMap**  
It is a auxiliary class.
```cpp
typedef std::map<char, FreeTypeCharacter> CharacterMap;
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

**characters**  
```cpp
std::map<FontSizePair, CharacterMap> characters;
```

**textShader**  
```cpp
Render::Shader textShader;
```

### Functions:
**Public:**  
**Init**  
It initializes the `Text2DRenderer`.  
Note: It should be called after `RenderManager`.
```cpp
void Init(const Render::Resolution& resolution);
```

**Terminate**  
It terminates the `Text2DRenderer`. 
Note: It should be terminated before `RenderManager`.
```cpp
void Terminate();
```

**Load**  
They load the first `FONT_NUMBER` characters from a ttf file with a size.
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

**RenderText**  
It renders the text.  
Note: The blend should be enabled,
where the source factor is *GL_SRC_ALPHA*
and destination factor is *GL_ONE_MINUS_SRC_ALPHA*
```cpp
void RenderText(const Text& text);
```

