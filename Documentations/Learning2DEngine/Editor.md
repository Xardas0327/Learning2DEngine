# Editor
The engine does not have its own editor.  
But the Tiled Map Editor's CSV export is supported.  
Supported format version: 1.10

- [BaseLayerItemData](System.md#baselayeritemdata)
- [BaseObjectData](System.md#baseobjectdata)
- [LayerItemData](System.md#layeritemdata)
- [ObjectBox](System.md#objectbox)
- [ObjectEllipse](System.md#objectellipse)
- [ObjectImage](System.md#objectimage)
- [ObjectItem](System.md#objectitem)
- [ObjectLayerItemData](System.md#objectlayeritemdata)
- [ObjectPoint](System.md#objectpoint)
- [ObjectType](System.md#objecttype)
- [TiledMap](System.md#tiledmap)
- [TiledMapConstant](System.md#tiledmapconstant)
- [TiledMapLoader](System.md#tiledmaploader)
- [TiledMapTileset](System.md#tiledmaptileset)

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


