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
