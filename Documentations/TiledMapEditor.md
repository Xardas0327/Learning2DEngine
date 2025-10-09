# Tiled Map Editor

The engine does not have its own editor.
That is why the [Tiled Map Editor](https://www.mapeditor.org/) is supported.  
This user documentation will help, Tiled Map Editor's feature are supported
and which are not.  
If you are interested more about the code, it is [here](Learning2DEngine/Editor.md).  
Here the code sample will be minimal.

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

## Layer

## Object Layer

## Object

## Tileset

## Tile

## Tile Object

## Properties


