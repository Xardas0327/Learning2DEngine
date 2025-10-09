# Editor
The engine does not have its own editor.  
But the Tiled Map Editor's CSV export is supported.  
Supported format version: 1.10

- [BaseLayerItemData](Editor.md#baselayeritemdata)
- [BaseObjectData](Editor.md#baseobjectdata)
- [LayerItemData](Editor.md#layeritemdata)
- [ObjectBox](Editor.md#objectbox)
- [ObjectEllipse](Editor.md#objectellipse)
- [ObjectImage](Editor.md#objectimage)
- [ObjectItem](Editor.md#objectitem)
- [ObjectLayerItemData](Editor.md#objectlayeritemdata)
- [ObjectPoint](Editor.md#objectpoint)
- [ObjectType](Editor.md#objecttype)
- [TiledMap](Editor.md#tiledmap)
- [TiledMapConstant](Editor.md#tiledmapconstant)
- [TiledMapLoader](Editor.md#tiledmaploader)
- [TiledMapTileset](Editor.md#tiledmaptileset)

##
## BaseLayerItemData
### Source Code:
[BaseLayerItemData.h](../../Learning2DEngine/Learning2DEngine/Editor/BaseLayerItemData.h)  

### Description:
It is a base class for the layer data.  
Please use the `LayerItemData` or `ObjectLayerItemData` classes.

### Header:
```cpp
class BaseLayerItemData
{...}
```

### Variables:
**Public**  
**layerId**  
```cpp
int layerId;
```

**visible**  
```cpp
bool visible;
```

**opacity**  
```cpp
float opacity;
```

**offset**  
```cpp
glm::vec2 offset;
```

**tintColor**  
```cpp
glm::vec4 tintColor;
```

**tilesets**  
```cpp
const std::vector<TiledMapTileset>& tilesets;
```

### Functions:
**Protected:**  
**BaseLayerItemData**  
```cpp
BaseLayerItemData(const std::vector<TiledMapTileset>& tilesets, int layerId = 0, bool visible = true, float opacity = 1.0f, glm::vec2 offset = glm::vec2(0.0f), glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
```

**Public:**  
**~BaseLayerItemData**  
```cpp
virtual ~BaseLayerItemData() = default;
```

##
## BaseObjectData
### Source Code:
[BaseObjectData.h](../../Learning2DEngine/Learning2DEngine/Editor/BaseObjectData.h)  

### Description:
It is a base class for the object data.  
Please use the `ObjectPoint`, `ObjectBox`, `ObjectEllipse`, `ObjectImage` or 
`ObjectItem` classes.

### Header:
```cpp
class BaseObjectData
{...}
```

### Variables:
**Public**   
**visible**  
```cpp
bool visible;
```

**properties**  
```cpp
std::map<std::string, System::Property> properties;
```

### Functions:
**Protected:**  
**BaseLayerItemData**  
```cpp
BaseObjectData(std::map<std::string, System::Property>&& properties, bool visible = true);
```

##
## LayerItemData
### Source Code:
[LayerItemData.h](../../Learning2DEngine/Learning2DEngine/Editor/LayerItemData.h)  

### Description:
It contains the tile layer data, when the TiledMapLoader create a `GameObject`.

### Header:
```cpp
class LayerItemData : public BaseLayerItemData
{...}
```

### Variables:
**Public**  
**gid**  
```cpp
int gid;
```

**row**  
The row index in the tile layer.  
Note: The first row is 0.
```cpp
int row;
```

**column**  
The column index in the tile layer.
Note: The first column is 0.
```cpp
int column;
```

### Functions:
**Public:**  
**LayerItemData**  
```cpp
LayerItemData(const std::vector<TiledMapTileset>& tilesets, int gid, int row, int column, int layerId = 0, bool visible = true, float opacity = 1.0f, glm::vec2 offset = glm::vec2(0.0f), glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
```

##
## ObjectBox
### Source Code:
[ObjectBox.h](../../Learning2DEngine/Learning2DEngine/Editor/ObjectBox.h)  

### Description:
It is the representation of a box object from Tiled Map Editor.  
A `BoxColliderComponent` will be created from this object.  
Note: If the visible is false, the `BoxColliderComponent` won't be active, but
the `GameObject` will be active.    

### Header:
```cpp
struct ObjectBox : public ObjectPoint
{...}
```

### Variables:
**Public**  
**size**  
```cpp
glm::vec2 size;
```

### Functions:
**Public:**  
**ObjectBox**  
```cpp
ObjectBox(const glm::vec2& position, const glm::vec2& size, std::map<std::string, System::Property>&& properties, bool visible = true);
```

##
## ObjectEllipse
### Source Code:
[ObjectEllipse.h](../../Learning2DEngine/Learning2DEngine/Editor/ObjectEllipse.h)  

### Description:
It is the representation of an ellipse object from Tiled Map Editor.  
A `CircleColliderComponent` will be created from this object.  
Note: If the visible is false, the `CircleColliderComponent` won't be active, but
the `GameObject` will be active.  
Only circle is supported, not ellipse. If the width (x) and height (y)
are not equal, the width will be used.

### Header:
```cpp
struct ObjectEllipse : public ObjectPoint
{...}
```

### Variables:
**Public**  
**size**  
```cpp
glm::vec2 size;
```

### Functions:
**Public:**  
**ObjectEllipse**  
```cpp
ObjectEllipse(const glm::vec2& position, const glm::vec2& size, std::map<std::string, System::Property>&& properties, bool visible = true);
```

##
## ObjectImage
### Source Code:
[ObjectImage.h](../../Learning2DEngine/Learning2DEngine/Editor/ObjectImage.h)  

### Description:
It is the representation of an Image object from Tiled Map Editor.  
A `SpriteRenderComponent` will be created from this object.  
Note: If the visible is false, the `SpriteRenderComponent` won't be active, but
the `GameObject` will be active.  

### Header:
```cpp
struct ObjectImage : public ObjectBox
{...}
```

### Variables:
**Public**  
**gid**  
```cpp
int gid;
```

### Functions:
**Public:**  
**ObjectImage**  
```cpp
ObjectImage(const glm::vec2& position, const glm::vec2& size, std::map<std::string, System::Property>&& properties, int gid, bool visible = true);
```

##
## ObjectItem
### Source Code:
[ObjectItem.h](../../Learning2DEngine/Learning2DEngine/Editor/ObjectItem.h)  

### Description:
It is a container for different object types from Tiled Map Editor.

### Header:
```cpp
class ObjectItem final
{...}
```

### Variables:
**Private**  
**data**  
```cpp
BaseObjectData* data;
```

**Public**  
**type**  
```cpp
const ObjectType type;
```

### Functions:
**Public:**  
**ObjectItem**  
```cpp
ObjectItem(ObjectPoint&& point);
```
```cpp
ObjectItem(ObjectBox&& box);
```
```cpp
ObjectItem(ObjectEllipse&& ellipse);
```
```cpp
ObjectItem(ObjectImage&& image);
```
```cpp
ObjectItem(const ObjectItem& other);
```

**~ObjectItem**  
```cpp
~ObjectItem();
```

**GetData**  
```cpp
inline const BaseObjectData* GetData() const;
```

##
## ObjectLayerItemData
### Source Code:
[ObjectLayerItemData.h](../../Learning2DEngine/Learning2DEngine/Editor/ObjectLayerItemData.h)  

### Description:
It contains the object layer data, when the TiledMapLoader create a `GameObject`.

### Header:
```cpp
class ObjectLayerItemData : public BaseLayerItemData
{...}
```

### Variables:
**Public**  
**objectItem**  
```cpp
const ObjectItem& objectItem;
```

### Functions:
**Public:**  
**ObjectLayerItemData**  
```cpp
ObjectLayerItemData(const std::vector<TiledMapTileset>& tilesets, const ObjectItem& objectItem, int layerId = 0, bool visible = true, float opacity = 1.0f, glm::vec2 offset = glm::vec2(0.0f), glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
```

##
## ObjectPoint
### Source Code:
[ObjectPoint.h](../../Learning2DEngine/Learning2DEngine/Editor/ObjectPoint.h)  

### Description:
It is the representation of a point object from Tiled Map Editor.  
A `GameObject` will be created from this object.  
Note: If the visible is false, the `GameObject` won't be active.  

### Header:
```cpp
struct ObjectPoint : public BaseObjectData
{...}
```

### Variables:
**Public**  
**position**  
```cpp
glm::vec2 position;
```

### Functions:
**Public:**  
**ObjectBox**  
```cpp
ObjectPoint(const glm::vec2& position, std::map<std::string, System::Property>&& properties, bool visible = true);
```

##
## ObjectType
### Source Code:
[ObjectType.h](../../Learning2DEngine/Learning2DEngine/Editor/ObjectType.h)  

### Description:
The supported object types.

### Header:
```cpp
enum class ObjectType
{
	POINT,
	BOX,
	ELLIPSE,
	IMAGE
};
```

##
## TiledMap
### Source Code:
[TiledMap.h](../../Learning2DEngine/Learning2DEngine/Editor/TiledMap.h)  

### Description:
It contains the Tiled Map data and the game objects, which were created.

### Header:
```cpp
class TiledMap final
{...}
```

### Variables:
**Private**  
**version**  
```cpp
std::string version;
```

**tiledVersion**  
```cpp
std::string tiledVersion;
```

**orientation**  
```cpp
std::string orientation;
```

**width**  
```cpp
int width;
```

**height**  
```cpp
int height;
```

**tileWidth**  
```cpp
int tileWidth;
```

**tileHeight**  
```cpp
int tileHeight;
```

**infinite**  
```cpp
bool infinite;
```

**backgroundColor**  
```cpp
glm::vec4 backgroundColor;
```

**Public**  
**gameObjects**  
It contains all game object, which are not in a group.
```cpp
std::vector<System::GameObject*> gameObjects;
```

**groupedGameObjects**  
It contains those game objects, which are in a group.
```cpp
std::map<std::string, std::vector<System::GameObject*>> groupedGameObjects;
```

### Functions:
**Private:**  
**TiledMap**  
```cpp
TiledMap();
```

**Public**  
**~TiledMap**  
```cpp
~TiledMap() = default;
```

**GetVersion**  
```cpp
inline const std::string& GetVersion() const;
```

**GetTiledVersion**  
```cpp
inline const std::string& GetTiledVersion() const;
```

**GetOrientation**  
```cpp
inline const std::string& GetOrientation() const;
```

**GetWidth**  
```cpp
inline int GetWidth() const;
```

**GetHeight**  
```cpp
inline int GetHeight() const;
```

**GetTileWidth**  
```cpp
inline int GetTileWidth() const;
```

**GetTileHeight**  
```cpp
inline int GetTileHeight() const;
```

**IsInfinite**  
```cpp
inline bool IsInfinite() const;
```

**GetBackgroundColor**  
```cpp
inline const glm::vec4& GetBackgroundColor() const;
```

##
## TiledMapConstant
### Source Code:
[TiledMapConstant.h](../../Learning2DEngine/Learning2DEngine/Editor/TiledMapConstant.h)  

### Description:
These constans are used in the TiledMapLoader.  
They are node names and attribute names and some values.


### Variables:
**Supported versions**  
```cpp
constexpr const char* TiledMapSupportedVersion = "1.10";
constexpr const char* TiledMapSupportedOrientation = "orthogonal";
constexpr const char* TiledMapSupportedEncoding = "csv";
```

**Common**  
These constants are used in multiple places.
```cpp
constexpr const char* TiledMapAttrName = "name";
constexpr const char* TiledMapAttrWidth = "width";
constexpr const char* TiledMapAttrHeight = "height";
constexpr const char* TiledMapAttrTileWidth = "tilewidth";
constexpr const char* TiledMapAttrTileHeight = "tileheight";
constexpr const char* TiledMapAttrVisible = "visible";
constexpr const char* TiledMapAttrOpacity = "opacity";
constexpr const char* TiledMapAttrTintcolor = "tintcolor";
constexpr const char* TiledMapAttrOffsetX = "offsetx";
constexpr const char* TiledMapAttrOffsetY = "offsety";
constexpr const char* TiledMapAttrX = "x";
constexpr const char* TiledMapAttrY = "y";
```

**Map**  
```cpp
constexpr const char* TiledMapNodeMap = "map";
constexpr const char* TiledMapAttrVersion = "version";
constexpr const char* TiledMapAttrTiledVersion = "tiledversion";
constexpr const char* TiledMapAttrOrientation = "orientation";
constexpr const char* TiledMapAttrInfinite = "infinite";
constexpr const char* TiledMapAttrBackgroundColor = "backgroundcolor";
```

**Layer**  
```cpp
constexpr const char* TiledMapNodeLayer = "layer";
constexpr const char* TiledMapNodeData = "data";
constexpr const char* TiledMapAttrEncoding = "encoding";
```

**Tileset**  
```cpp
constexpr const char* TiledMapNodeTileset = "tileset";
constexpr const char* TiledMapNodeImage = "image";
constexpr const char* TiledMapAttrFirstGid = "firstgid";
constexpr const char* TiledMapAttrSource = "source";
constexpr const char* TiledMapAttrColumns = "columns";
constexpr const char* TiledMapAttrTileCount = "tilecount";
constexpr const char* TiledMapAttrSpacing = "spacing";
constexpr const char* TiledMapAttrMargin = "margin";
constexpr const char* TiledMapAttrTileOffset = "tileoffset";
```

**Tile**  
```cpp
constexpr const char* TiledMapNodeTile = "tile";
constexpr const char* TiledMapAttrId = "id";
```

**Object Group**  
```cpp
constexpr const char* TiledMapNodeObjectGroup = "objectgroup";
constexpr const char* TiledMapNodeObject = "object";
constexpr const char* TiledMapNodePoint = "point";
constexpr const char* TiledMapNodeEllipse = "ellipse";
constexpr const char* TiledMapNodePolygon = "polygon";
constexpr const char* TiledMapNodeText = "text";
constexpr const char* TiledMapAttrGid = "gid";
```

**Property**  
```cpp
constexpr const char* TiledMapNodeProperties = "properties";
constexpr const char* TiledMapNodeProperty = "property";
constexpr const char* TiledMapAttrType = "type";
constexpr const char* TiledMapAttrValue = "value";
constexpr const char* TiledMapPropertyTypeBool = "bool";
constexpr const char* TiledMapPropertyTypeColor = "color";
constexpr const char* TiledMapPropertyTypeFile = "file";
constexpr const char* TiledMapPropertyTypeFloat = "float";
constexpr const char* TiledMapPropertyTypeInt = "int";
constexpr const char* TiledMapPropertyTypeString = "string";
constexpr const char* TiledMapPropertyTypeObject = "object";
```

**Smart Property**  
If these properties are used, the TiledMapLoader will do something.  
For example: load background color, add colliders etc.
```cpp
constexpr const char* TiledMapSmartLoadBackground = "LoadBackground";
constexpr const char* TiledMapSmartLayer = "Layer";
constexpr const char* TiledMapSmartGroupName = "GroupName";
constexpr const char* TiledMapSmartOnGameObject = "OnGameObject";
constexpr const char* TiledMapSmartCollider = "Collider";
constexpr const char* TiledMapSmartColliderValueBox = "Box";
constexpr const char* TiledMapSmartColliderValueCircle = "Circle";
constexpr const char* TiledMapSmartColliderSizeX = "SizeX";
constexpr const char* TiledMapSmartColliderSizeY = "SizeY";
constexpr const char* TiledMapSmartColliderRadius = "Radius";
constexpr const char* TiledMapSmartColliderIsKinematic = "IsKinematic";
constexpr const char* TiledMapSmartColliderIsTrigger = "IsTrigger";
constexpr const char* TiledMapSmartColliderOffsetX = "OffsetX";
constexpr const char* TiledMapSmartColliderOffsetY = "OffsetY";
constexpr const char* TiledMapSmartColliderMaskLayer = "MaskLayer";
```

##
## TiledMapLoader
### Source Code:
[TiledMapLoader.h](../../Learning2DEngine/Learning2DEngine/Editor/TiledMap.h)  
[TiledMapLoader.cpp](../../Learning2DEngine/Learning2DEngine/Editor/TiledMapLoader.cpp)  

### Description:
This is the main class to load a Tiled Map Editor map from the main tmx file.  
All functions are static.

### Header:
```cpp
class TiledMapLoader final
{...}
```

### Functions:
**Private:**  
**TiledMapLoader**  
```cpp
TiledMapLoader() = default;
```

**LoadMapAttributes**  
```cpp
static void LoadMapAttributes(TiledMap& map, rapidxml::xml_node<>* mapNode);
```

**ConvertStringToColor**  
It converts a hexadecimal string to a glm::vec4 color.
```cpp
static glm::vec4 ConvertStringToColor(const std::string& hex);
```

**LoadTilesets**  
```cpp
static std::vector<TiledMapTileset> LoadTilesets(rapidxml::xml_node<>* mapNode, const std::string& folderPath, const std::map<std::string, std::string>& textureMap);
```

**LoadTileset**  
The LoadTilesets function use this one.
```cpp
static bool LoadTileset(const std::string& folderPath, const std::string& sourceName, const std::map<std::string, std::string>& textureMap, TiledMapTileset& tiledMapObject);
```

**LoadLayers**  
```cpp
static void LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, std::vector<TiledMapTileset>& tilesets);
```

**LoadObjectLayers**  
```cpp
static void LoadObjectLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, std::vector<TiledMapTileset>& tilesets, const std::string& folderPath = "");
```

**LoadProperties**  
The folderPath is used when the property type is file.
```cpp
static std::map<std::string, System::Property> LoadProperties(rapidxml::xml_node<>* node, const std::string& folderPath = "");
```

**LoadTilesProperties**  
The folderPath is used when the property type is file.  
The sourceName is used for the logging only.
```cpp
static std::map<int, std::map<std::string, System::Property>> LoadTilesProperties(rapidxml::xml_node<>* node, const std::string& sourceName, const std::string& folderPath = "");
```

**LoadObjectItems**  
The folderPath is used when the property type is file.  
```cpp
static std::vector<ObjectItem> LoadObjectItems(rapidxml::xml_node<>* node, const std::string& folderPath = "");
```

**LoadTilesObjectItems**  
The folderPath is used when the property type is file.  
The sourceName is used for the logging only.
```cpp
static std::map<int, std::vector<ObjectItem>> LoadTilesObjectItems(rapidxml::xml_node<>* node, const std::string& sourceName, const std::string& folderPath = "");
```

**LoadMapBackground**  
```cpp
static bool LoadMapBackground(rapidxml::xml_node<>* mapNode);
```

**GetTilesetFromGid**  
```cpp
static const TiledMapTileset* GetTilesetFromGid(const std::vector<TiledMapTileset>& tilesets, int gid);
```

**CreateGameObject**  
```cpp
static void CreateGameObject(TiledMap& map, const LayerItemData& itemData);
```
```cpp
static void CreateGameObject(TiledMap& map, const ObjectLayerItemData& itemData);
```

**AddColliderToGameObject**  
```cpp
static void AddColliderToGameObject(System::GameObject* gameObject, std::map<std::string, System::Property>& properties);
```

**CreateColliderFromObjectItem**  
```cpp
template<class T>
static void CreateColliderFromObjectItem(ObjectItem objectItem, System::GameObject* tiledGameObject, std::map<std::string, System::Property>& tiledProperties);
```

**AddColliderToGameObject**  
The object.position will be the offset if the useObjectPositionAsOffset is true.
```cpp
static void AddColliderToGameObject(System::GameObject* gameObject, const ObjectBox& object, std::map<std::string, System::Property>& properties, bool useObjectPositionAsOffset);
```
```cpp
static void AddColliderToGameObject(System::GameObject* gameObject, const ObjectEllipse& object, std::map<std::string, System::Property>& properties, bool useObjectPositionAsOffset);
```

**Public:**  
**~TiledMapLoader()**  
```cpp
~TiledMapLoader() = default;
```

**LoadFromFile()**  
```cpp
static TiledMap LoadFromFile(const std::string& filePath, const std::map<std::string, std::string>& textureMap = std::map<std::string, std::string>());
```

##
## TiledMapTileset
### Source Code:
[TiledMapTileset.h](../../Learning2DEngine/Learning2DEngine/Editor/TiledMapTileset.h)  

### Description:
It contains the tileset data.

### Header:
```cpp
struct TiledMapTileset final
{...}
```

### Variables:
**Public**  
**firstGid**  
```cpp
int firstGid;
```

**columns**  
```cpp
int columns;
```

**tileCount**  
```cpp
int tileCount;
```

**spacing**  
```cpp
int spacing;
```

**margin**  
```cpp
int margin;
```

**tileSize**  
```cpp
glm::vec2 tileSize;
```

**imageSize**  
```cpp
glm::vec2 imageSize;
```

**tileOffset**  
```cpp
glm::vec2 tileOffset;
```

**texture**  
```cpp
Render::Texture2D* texture;
```

**commonProperties**  
```cpp
std::map<std::string, System::Property> commonProperties;
```

**uniqueProperties**  
```cpp
std::map<int, std::map<std::string, System::Property>> uniqueProperties;
```

**objects**  
```cpp
std::map<int, std::vector<ObjectItem>> objects;
```

### Functions:
**Public:**  
**TiledMapTileset**  
```cpp
TiledMapTileset();
```

**~TiledMapTileset**  
```cpp
~TiledMapTileset() = default;
```

**HasNumber**  
```cpp
bool HasNumber(int gid) const;
```

**GetLocalId**  
```cpp
int GetLocalId(int gid) const;
```

**GetUV**  
The texture coordinate order:  
Top Left,  
Top Right,  
Bottom Right,  
Bottom Left
```cpp
glm::mat4x2 GetUV(int gid) const;
```


