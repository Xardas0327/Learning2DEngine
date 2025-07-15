# UI
- [FontSizePair](UI.md#fontsizepair)
- [FreeTypeCharacter](UI.md#freetypecharacter)
- [MultiText2DRenderer](UI.md#multitext2drenderer)
- [SimpleText2DRenderComponent](UI.md#simpletext2drendercomponent)
- [SimpleText2DRenderer](UI.md#simpletext2drenderer)
- [Text2DRenderComponent](UI.md#text2drendercomponent)
- [Text2DDynamicData](UI.md#text2ddynamicdata)
- [Text2DRenderData](UI.md#text2drenderdata)
- [TextBoxComponent](UI.md#textboxcomponent)
- [TextBoxMode](UI.md#textboxmode)
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
class MultiText2DRenderer : public Render::BaseMultiRenderer<Text2DDynamicData>, public System::Singleton<MultiText2DRenderer>
{...}
```

### Variables:
**Private:**  
**textRenderData**  
The first int is the layer. 
The map key is a GLuint, which a character texture id, and the value is a vector of array,
which contains the position, the texture coordinates, the color and isUseCameraView.
```cpp
std::map<int, std::map<GLuint, std::vector<std::array<float, 33>>>> textRenderData;
```

### Functions:
**Private:**  
**MultiText2DRenderer**  
```cpp
MultiText2DRenderer();
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

**CalcDynamicDataSize**  
If the current buffers' size is not enough or it is bigger twice than the maxDynamicSize,
it will reallocate the buffers.  
It allocates 20% more space in the buffer, so that it does not have to allocate again 
if there are some dynamic renderers.
```cpp
void CalcDynamicDataSize(size_t maxDynamicSize) override;
```

**Public:**  
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
## SimpleText2DRenderComponent
### Source Code:
[SimpleText2DRenderComponent.h](../../Learning2DEngine/Learning2DEngine/UI/SimpleText2DRenderComponent.h)  
[SimpleText2DRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/UI/SimpleText2DRenderComponent.cpp)

### Description:
It can render a text with a color.  
It uses `SimpleText2DRenderer` for rendering. It is recommand, when the developer knows,
there are only little texts.  
Please more info about `RendererComponent`.

### Header:
```cpp
class SimpleText2DRenderComponent : public Render::RendererComponent<Text2DRenderData, SimpleText2DRenderer>
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
**SimpleText2DRenderComponent**  
```cpp
SimpleText2DRenderComponent(System::GameObject* gameObject, Render::RendererMode mode, const FontSizePair& fontSizePair, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
SimpleText2DRenderComponent(System::GameObject* gameObject, Render::RendererMode mode, const FontSizePair& fontSizePair, const std::string& text, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetInitedRenderer**  
```cpp
SimpleText2DRenderer* GetInitedRenderer() override;
```

**DestroyRenderer**  
```cpp
void DestroyRenderer() override;
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
class SimpleText2DRenderer : public Render::BaseRenderer, public System::Singleton<SimpleText2DRenderer>
{...}
```

### Variables:
**Private:**  
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
void SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData) override;
```

**Draw**  
It draws those objects, which was added with SetData and they are on the selected layer.
```cpp
void Draw(int layer) override;
```


##
## Text2DRenderComponent
### Source Code:
[Text2DRenderComponent.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DRenderComponent.h)  
[Text2DRenderComponent.cpp](../../Learning2DEngine/Learning2DEngine/UI/Text2DRenderComponent.cpp)

### Description:
It can render a text with a color.  
It uses `MultiText2DRenderer` for rendering. It is recommand, when the developer knows,
there are huge texts.  
Please more info about `RendererComponent`.

### Header:
```cpp
class Text2DRenderComponent : public Render::RendererComponent<Text2DRenderData, MultiText2DRenderer>
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
**Text2DLateRenderComponent**  
```cpp
Text2DRenderComponent(System::GameObject* gameObject, Render::RendererMode mode, const FontSizePair& fontSizePair, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
Text2DRenderComponent(System::GameObject* gameObject, Render::RendererMode mode, const FontSizePair& fontSizePair, const std::string& text, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetInitedRenderer**  
```cpp
MultiText2DRenderer* GetInitedRenderer() override;
```

**DestroyRenderer**  
```cpp
void DestroyRenderer() override;
```

##
## Text2DDynamicData
### Source Code:
[Text2DDynamicData.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DDynamicData.h)   

### Description:
It contains the dynamic data of the `MultiText2DRenderer`.  
Note: the textureId and the isUseCameraView are float, because we sent it to vertex shader,
but textureId will be converted to int and the isUseCameraView will be converted to bool.

### Header:
```cpp
struct Text2DDynamicData
{
	float position[2];
	float textCoord[2];
	float color[4];
	float textureId;
	float isUseCameraView;
};
```

##
## Text2DRenderData
### Source Code:
[Text2DRenderData.h](../../Learning2DEngine/Learning2DEngine/UI/Text2DRenderData.h)  
[Text2DRenderData.cpp](../../Learning2DEngine/Learning2DEngine/UI/Text2DRenderData.cpp)  

### Description:
It contains the data, which is important to render a text.

### Header:
```cpp
class Text2DRenderData : public Render::RenderData
{...}
```

### Variables:
**Protected:**  
**fontSizePair**
```cpp
FontSizePair fontSizePair;
```

**text**
```cpp
std::string text;
```

**shouldRecalcSize**
```cpp
bool shouldRecalcSize;
```

**shouldRecalcVertices**
```cpp
bool shouldRecalcVertices;
```

**previousModelMatrix**
```cpp
glm::mat4 previousModelMatrix;
```

**characterVertices**
```cpp
std::map<char, std::vector<glm::mat4>> characterVertices;
```

**textSize**
```cpp
glm::vec2 textSize;
```

**Public:**  
**color**
```cpp
glm::vec4 color;
```

**isUseCameraView**  
It shows, that the text should be rendered with camera view matrix or not.  
Note: default value is false.
```cpp
bool isUseCameraView;
```

### Functions:
**Protected:**  
**CalculateTextSize**
```cpp
glm::vec2 CalculateTextSize() const;
```

**CalculateCharacterVertices**
```cpp
std::map<char, std::vector<glm::mat4>> CalculateCharacterVertices() const;
```

**GetRotationMatrix**
```cpp
glm::mat2 GetRotationMatrix() const;
```

**Public:**  
**Text2DRenderData**
```cpp
Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, const std::string& text, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, const std::string& text, bool isUseCameraView, glm::vec4 color = glm::vec4(1.0f));
```

**GetText**
```cpp
inline const std::string& GetText() const;
```

**SetText**
```cpp
void SetText(const std::string& text);
```
```cpp
void SetText(std::string&& text);
```

**GetFontSizePair**
```cpp
inline const FontSizePair& GetFontSizePair() const;
```

**SetFontSizePair**
```cpp
void SetFontSizePair(const FontSizePair& fontSizePair);
```

**GetCharacterVertices**  
If the shouldRecalcVertices is true or the previousModelMatrix is not the same as
the current modelMatrix, the non const version save the new calculated character vertices,
that's why it will not recalculate the character vertices again.  
In same situation the const version will always recalculate the character vertices.  
Note: The previous model matrix is used to track the gameobject's transform has changed or not.
```cpp
const std::map<char, std::vector<glm::mat4>>& GetCharacterVertices();
```
```cpp
std::map<char, std::vector<glm::mat4>> GetCharacterVertices() const;
```

**GetTextSize**  
If the shouldRecalcSize is true the non const version save the new calculated text size,
that's why it will not recalculate the text size again.  
In same situation the const version will always recalculate the text size.
```cpp
glm::vec2 GetTextSize();
```
```cpp
glm::vec2 GetTextSize() const;
```

##
## TextBoxComponent
### Source Code:
[TextBoxComponent.h](../../Learning2DEngine/Learning2DEngine/UI/TextBoxComponent.h)  
[TextBoxComponent.cpp](../../Learning2DEngine/Learning2DEngine/UI/TextBoxComponent.cpp)

### Description:
A text box component, which can be used to display a text in a box.  
The `TextBoxComponent` will create a new GameObject with a `SimpleSpriteRenderComponent`
or with a `SpriteRenderComponent`.  
The render component will be updated in a LateUpdate().
So, if the text should be updated in a Update() function(s).

### Header:
```cpp
class TextBoxComponent : public System::LateUpdaterComponent
{...}
```

### Variables:
**Protected:**  
**textRenderData**  
```cpp
Text2DRenderData* const textRenderData;
```

**simpleRenderComponent**  
This or the multiRenderComponent will be used to render the box.
```cpp
Render::SimpleSpriteRenderComponent* simpleRenderComponent;
```

**multiRenderComponent**  
This or the simpleRenderComponent will be used to render the box.
```cpp
Render::SpriteRenderComponent* multiRenderComponent;
```

**initLayer**  
```cpp
const int initLayer;
```

**initColor**  
```cpp
const glm::vec4 initColor;
```

**initRendererMode**  
```cpp
const Render::RendererMode initRendererMode;
```

**textBoxMode**  
```cpp
const TextBoxMode textBoxMode;
```

**Public:**  
**paddingTopBottom**  
```cpp
float paddingTopBottom = 0.0f;
```

**paddingLeftRight**  
```cpp
float paddingLeftRight= 0.0f;
```

### Functions:
**Protected:**  
**TextBoxComponent**  
```cpp
TextBoxComponent(System::GameObject* gameObject, SimpleText2DRenderComponent& textComponent, TextBoxMode textBoxMode = TextBoxMode::SIMPLE, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```
```cpp
TextBoxComponent(System::GameObject* gameObject, Text2DRenderComponent& textComponent, TextBoxMode textBoxMode = TextBoxMode::SIMPLE, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```

**Init**  
```cpp
void Init() override;
```

**Destroy**  
```cpp
void Destroy() override;
```

**LateUpdate**  
```cpp
void LateUpdate() override;
```

**Public:**  
**SetPadding**  
It set the padding for all sides.  
```cpp
void SetPadding(float padding);
```

**GetLayer**  
```cpp
int GetLayer() const;
```

**SetLayer**  
```cpp
void SetLayer(int layer);
```

**GetColor**  
```cpp
glm::vec4 GetColor() const;
```

**SetColor**  
```cpp
void SetColor(const glm::vec4& color);
```

##
## TextBoxMode
### Source Code:
[TextBoxMode.h](../../Learning2DEngine/Learning2DEngine/UI/TextBoxMode.h)  

### Description:
The modes of the `TextBoxComponent`.

### Header:
```cpp
enum class TextBoxMode
{
	// The Textbox wil be rendered by the SimpleSpriteRenderComponent.
	SIMPLE,
	// The Textbox wil be rendered by the SpriteRenderComponent.
	MULTI
};
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
