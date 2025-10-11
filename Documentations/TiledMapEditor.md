# Tiled Map Editor

The engine does not have its own editor.
That is why the [Tiled Map Editor](https://www.mapeditor.org/) is supported.  
This user documentation will help, Tiled Map Editor's feature are supported
and which are not.  
If you are interested more about the code, it is [here](Learning2DEngine/Editor.md).  
Here the code sample will be minimal.  
If something is not supported, maybe it will be supported in the future.

Supported format version: 1.10

## How to use it in your game
If the `TiledMapLoader::LoadFromFile` function is called with a tmx file path,
then it will return a TiledMap object, which contains the map data and the loaded
game objects.
```cpp
TiledMap map = TiledMapLoader::LoadFromFile("Assets/Maps/Map.tmx");
```

The game objects are in the `map.gameObjects` or `map.groupedGameObjects`
if they were added to a group.  
The groupedGameObjects will be explained in the Properties section.

##
## Property
The Tiled Map Editor supports properties for almost everything.
It is a useful feature, because a tile, an object or even the layers or the map 
can have properties. Furthermore, the engine supports `Smart Properties`,
which are special properties.  
For example: overwrite the layer number or a collider will be kinematic etc.  
Every used `Smart Properties` will be removed from the properties list, and
the remaining properties will be added to the game object with a `PropertyComponent`.  
Every section will have a Property section, which will explain,
how the properties/smart properties are supported.

| Property Type | Supported |
| ------------- | --------- |
| Bool          | Yes       |
| Color         | Yes       |
| Float         | Yes       |
| File          | Yes       |
| Int           | Yes       |
| Object        | No        |
| String        | Yes       |

##
## Map
It is represented by the `TiledMap` class, but there is no game object for it.

| Attribute   | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Class       | No         | It is not relevant.|
| Orientation | Yes (part) | Only the `Orthogonal` is supported. |
| Width       | Yes        | |
| Height      | Yes        | |
| Tile Width  | Yes        | |
| Tile Height | Yes        | |
| Infinite    | No         | |
| Tile Side Length (Hex) | No | Only for hexagonal maps. |
| Stragger Axis  | No         | For staggered and hexagonal maps. |
| Stragger Index | No         | For staggered and hexagonal maps. |
| Parallax Origin | No         | |
| Tile Layer Format | Yes (part) | Only the `CSV` is supported. |
| Output Chucks | No | Only for Infinite maps. |
| Tile Render Order | No  | It is not relevant for the Engine. |
| Compression Level | No  | It is not relevant for CSV format. |
| Background Color  | Yes | It is used only by `Smart Property`: _LoadBackground_ |

#### Property
The engine doesn't support properties for the map, because it won't be a game object.  
But there is a `Smart Properties`, which is relevant for the map.

| Smart Property   | Type   | Description                                      |
| ---------------- | ------ | ------------------------------------------------ |
| LoadBackground   | Bool   | If it is true, the Background Color will set to `RenderManager::clearColor`|

##
## Layer
It doesn't have its own game object and it doesn't really have any representation.  
But its attribute affect game objects, which are in this layer.
Plus, every `BaseRendererComponent::layer` is depend on the layer order.
The first layer will be layer 0, the second layer 1 and so on.  
But the layer number/order can be overwritten by the `Smart Property`: _Layer_.

| Attribute   | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Name        | No         | It is not relevant.|
| Class       | No         | It is not relevant.|
| Visible     | Yes        | If it is false, the game objects will be inactive.|
| Locked      | No         | It is not relevant.|
| Opacity     | Yes        | `SpriteRenderComponent::data.color` alpha channel will be multiplied by it. (after the Tint Color)|
| Tint Color  | Yes        | `SpriteRenderComponent::data.color` will be overwritten by it (before Opacity).|
| Horizontal Offset | Yes  | It will be added to game object's position.|
| Vertical Offset | Yes   | It will be added to game object's position.|
| Parallax Factor | No    |   |
| Flipping by button | No | In the file, the tile id will be changed. |
| Rotate by button | No   | In the file, the tile id will be changed. |

#### Property
The engine doesn't support properties for the layers, because it won't be a game object.  
But there is a `Smart Properties`, which is relevant for the layers.

| Smart Property | Type  | Description   |
| -------------- | ----- | ------------- |
| Layer          | Int   | The `BaseRendererComponent::layer` will be overwritten by it. |

##
## Object Layer
It is more or less same as the Layer, just there are objects instead of tiles and
there are few more not relevant attribute too.  
So, it doesn't have its own game object and it doesn't really have any representation.  
But its attribute affect game objects, which are in this layer.
Plus, every `BaseRendererComponent::layer` is depend on the layer order.
The first layer will be layer 0, the second layer 1 and so on.  
But the layer number/order can be overwrite by the `Smart Property`: _Layer_.

| Attribute   | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Name        | No         | It is not relevant.|
| Class       | No         | It is not relevant.|
| Visible     | Yes        | If it is false, the game objects will be inactive.|
| Locked      | No         | It is not relevant.|
| Opacity     | Yes        | `SpriteRenderComponent::data.color` alpha channel will be multiplied by it. (after the Tint Color)|
| Tint Color  | Yes        | `SpriteRenderComponent::data.color` will be overwritten by it (before Opacity).|
| Horizontal Offset | Yes  | It will be added to game object's position.|
| Vertical Offset | Yes   | It will be added to game object's position.|
| Parallax Factor | No    |   |
| Color           | No    | It is not relevant. It is good in the editor only. |
| Drawing         | No    | It is not relevant. |
| Flipping by button | No | In the file, the tile id will be changed. |
| Rotate by button | No   | In the file, the tile id will be changed. |

#### Property
It is same as the Layer, because it won't be a game object.  
But there is a `Smart Properties`, which is relevant for the layers.

| Smart Property | Type  | Description   |
| -------------- | ----- | ------------- |
| Layer          | Int   | The `BaseRendererComponent::layer` will be overwritten by it. |

##
## Object
The Object Layer contains the objects, which will be converted to game objects.

| Object Type | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Rectangle   | Yes        | It will be a game object with `BoxColliderComponent`. The collider attribute is addable by `Smart Properties`.|
| Point       | Yes        | It will be a simple game object.|
| Ellipse     | Yes (part) | It will be a game object with `CircleColliderComponent`. The collider attribute is addable by `Smart Properties`. It will use only the half of the width for radius.|
| Polygon     | No         |   |
| Image       | Yes        | It will be a game object with `SpriteRenderComponent`.|
| Template    | No         |   |
| Text    | No         | It is not supported, because it is not compatible with the engine's text renderers. |

#### Attributes
They have more or less same attributes.  
Just the point object doesn't have width and height.

| Attribute   | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Name        | No         | It is not relevant.|
| Class       | No         | It is not relevant.|
| Visible     | Yes        | If it is false, the game objects will be inactive.|
| X           | Yes        | |
| Y           | Yes        | |
| Width       | Yes        | |
| Height      | Yes (part) | The ellipse doesn't support it. |
| Rotation    | No         | It is not supported, because it is not compatible with the `Transform::rotation`. |
| Flipping by button | No |  |
| Rotate by button   | No |  |

#### Property
The object can have properties, which will be added to the game object.  
The Point Object supports only the _GroupName_ `Smart Properties`, but other object do.

| Smart Property | Type   | Description   |
| -------------- | ------ | ------------- |
| GroupName      | String | The game object will added to the `TiledMap::groupedGameObjects`, where the index is the _GroupName_'s value |
| Collider       | String | The Image Object only supports it. The _Collider_'s value can be only `Box` or `Circle` and the engine will add a `BoxColliderComponent` or a `CircleColliderComponent` to the game object.  |
| IsKinematic    | Bool   | If it is true, the collider type will be kinematic. Otherwise it will be dynamic. |
| IsTrigger      | Bool   | If it is true, the collider mode will be trigger. Otherwise it will be collider. |
| OffsetX        | Float  | The collider's offset.x will be it. |
| OffsetY        | Float  | The collider's offset.y will be it. |
| MaskLayer      | Int    | The collider's mask layer will be it. |
| SizeX          | Float  | If the collider is Box, the collider's size.x will be it. |
| SizeY          | Float  | If the collider is Box, the collider's size.y will be it. |
| Radius         | Float  | If the collider is Circle, the collider's radius will be it. |

##
## Tileset
A tileset contains some tiles, which are from the same image and
they have the same attributes and properties.  

| Import Settings   | Supported |
| ----------- | ---------- |
| Use transparent color | No         |
| Margin                | Yes        |
| Spacing               | Yes        |

| Attribute   | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Name        | No         | It is not relevant. |
| Class       | No         | It is not relevant. |
| Object Aligment   | No   |   |
| Tiled Render Size | No   |   |
| Fill Mode         | No   | It is relevant with Tiled Render Size only. |
| Background Color  | No   | It is relevant in Editor only. |
| Orientation       | No   | It is only used in case of isometric. |
| Grid Width        | Yes  |   |
| Grid Height       | Yes  |   |
| Allowed Transformations | No   |   |
| Rearrange | No   | It is not relevant. |

#### Property
The tileset can have properties, and support some `Smart Properties` too.  
Every tile will have those properties, which are in the tileset.  
That is why, if you want more about Tile Propertt, please check the Tile section.

##
## Tile
A tile is part of a tileset, which can be used
in the tile layer or as an image object.
When it is used in the tile layer, then it will also be a game object and 
it will have a `SpriteRenderComponent`.  
But it can have other components too with objects and `Smart Property`.  
Unfortunately, the anmation is not supported yet.

| Attribute   | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Class       | No         | It is not relevant.|
| Probability | No         | It is not relevant.|

#### Tile Object
They are really similar like the Object section. Just when it is added into a tile,
then it will be created every time, when the tile is used in the map. It can have
a separate game object or it can be added to the tile's game object by _OnGameObject_
`Smart Property`.  
If you interested more about the object's attributes, please check the Object section.

| Object Type | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Rectangle   | Yes        | It will be a game object with `BoxColliderComponent`. The collider attribute is addable by `Smart Properties`. It has separate game object by default, but with the _OnGameObject_ `Smart Property`, it will be added to the tile's game object.|
| Point       | Yes        | It will be a simple game object.|
| Ellipse     | Yes (part) | It will be a game object with `CircleColliderComponent`. The collider attribute is addable by `Smart Properties`. It will use only the half of the width for radius. It has separate game object by default, but with the _OnGameObject_ `Smart Property`, it will be added to the tile's game object.|
| Polygon     | No         |   |
| Template    | No         |   |
| Detect Bounding Box | Yes | It is a simple Rectangle with the tile's size. |

#### Property
The tiles and the tile's objects have property support.  
The tiles get the properties from more places:
1. The base properties from the tileset.
1. After that, the tile's own properties will be added. It a property has the same id,
	they will be overwritten.
1. After that, if a object will be added to the tile's game object by _OnGameObject_
	`Smart Property`, then the object's properties will be added too. 
	If a property has the same id, they will be overwritten.

Note: if a tile's object is not added to the tile's game object, then
It will work like a normal object.  
Note 2: the Point Object supports only the _GroupName_ `Smart Properties`.

| Smart Property | Type   | Description   |
| -------------- | ------ | ------------- |
| GroupName      | String | The game object will added to the `TiledMap::groupedGameObjects`, where the index is the _GroupName_'s value |
| OnGameObject   | Bool   | The supported object will be attached to game object.  |
| Collider       | String | The Tile only support it. The _Collider_'s value can be only `Box` or `Circle` and the engine will add a `BoxColliderComponent` or a `CircleColliderComponent` to the game object.  |
| IsKinematic    | Bool   | If it is true, the collider type will be kinematic. Otherwise it will be dynamic. |
| IsTrigger      | Bool   | If it is true, the collider mode will be trigger. Otherwise it will be collider. |
| OffsetX        | Float  | The collider's offset.x will be it. |
| OffsetY        | Float  | The collider's offset.y will be it. |
| MaskLayer      | Int    | The collider's mask layer will be it. |
| SizeX          | Float  | If the collider is Box, the collider's size.x will be it. |
| SizeY          | Float  | If the collider is Box, the collider's size.y will be it. |
| Radius         | Float  | If the collider is Circle, the collider's radius will be it. |



