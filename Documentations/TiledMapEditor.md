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

## Layer
It doesn't have its own game object and it doesn't really have any representation.  
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
| Flipping by button | No | In the file, the tile id will be changed. |
| Rotate by button | No   | In the file, the tile id will be changed. |


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
| Text    | No         | It is not supported, because it is not compatible with the engine's text renderers |

### Attributes
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

## Tileset

## Tile

## Tile Object

## Properties


