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

| Feature     | Supported  | Notes              |
| ----------- | ---------- | ------------------ |
| Class       | Yes        | It is not relevant. |
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
| Background Color  | Yes | It is used only by `Smart Property`: LoadBackground |

## Layer

## Object Layer

## Object

## Tileset

## Tile

## Tile Object

## Properties


