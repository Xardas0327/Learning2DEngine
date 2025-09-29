#pragma once


namespace Learning2DEngine
{
	namespace Editor
	{
		constexpr const char* TiledMapSupportedVersion = "1.10";
		constexpr const char* TiledMapSupportedOrientation = "orthogonal";
		constexpr const char* TiledMapSupportedEncoding = "csv";

		//Common
		constexpr const char* TiledMapAttrName = "name";
		constexpr const char* TiledMapAttrWidth = "width";
		constexpr const char* TiledMapAttrHeight = "height";
		constexpr const char* TiledMapAttrTileWidth = "tilewidth";
		constexpr const char* TiledMapAttrTileHeight = "tileheight";

		// Map
		constexpr const char* TiledMapNodeMap = "map";
		constexpr const char* TiledMapAttrVersion = "version";
		constexpr const char* TiledMapAttrTiledVersion = "tiledversion";
		constexpr const char* TiledMapAttrOrientation = "orientation";
		constexpr const char* TiledMapAttrInfinite = "infinite";
		constexpr const char* TiledMapAttrBackgroundColor = "backgroundcolor";

		// Tileset
		constexpr const char* TiledMapNodeTileset = "tileset";
		constexpr const char* TiledMapNodeImage = "image";
		constexpr const char* TiledMapAttrFirstGid = "firstgid";
		constexpr const char* TiledMapAttrSource = "source";
		constexpr const char* TiledMapAttrColumns = "columns";
		constexpr const char* TiledMapAttrTileCount = "tilecount";
		constexpr const char* TiledMapAttrSpacing = "spacing";
		constexpr const char* TiledMapAttrMargin = "margin";
	}
}

// Layer
#define L2DE_TILEDMAP_NODE_LAYER "layer"
#define L2DE_TILEDMAP_NODE_DATA "data"
#define L2DE_TILEDMAP_ATTR_ENCODING "encoding"

// Property
#define L2DE_TILEDMAP_NODE_PROPERTIES "properties"
#define L2DE_TILEDMAP_NODE_PROPERTY "property"
#define L2DE_TILEDMAP_ATTR_TYPE "type"
#define L2DE_TILEDMAP_ATTR_VALUE "value"
#define L2DE_TILEDMAP_PROPERTY_TYPE_BOOL "bool"
#define L2DE_TILEDMAP_PROPERTY_TYPE_COLOR "color"
#define L2DE_TILEDMAP_PROPERTY_TYPE_FILE "file"
#define L2DE_TILEDMAP_PROPERTY_TYPE_FLOAT "float"
#define L2DE_TILEDMAP_PROPERTY_TYPE_INT "int"
#define L2DE_TILEDMAP_PROPERTY_TYPE_STRING "string"
#define L2DE_TILEDMAP_PROPERTY_TYPE_OBJECT "object"

// Tile
#define L2DE_TILEDMAP_NODE_TILE "tile"
#define L2DE_TILEDMAP_ATTR_ID "id"

// Smart Property
#define L2DE_TILEDMAP_SMART_LOADBACKGROUND "LoadBackground"
#define L2DE_TILEDMAP_SMART_LAYER "Layer"
#define L2DE_TILEDMAP_SMART_GROUPNAME "GroupName"
#define L2DE_TILEDMAP_SMART_ONGAMEOBJECT "OnGameObject"
#define L2DE_TILEDMAP_SMART_COLLIDER "Collider"
#define L2DE_TILEDMAP_SMART_COLLIDER_VALUE_BOX "Box"
#define L2DE_TILEDMAP_SMART_COLLIDER_VALUE_CIRCLE "Circle"
#define L2DE_TILEDMAP_SMART_COLLIDER_SIZE_X "SizeX"
#define L2DE_TILEDMAP_SMART_COLLIDER_SIZE_Y "SizeY"
#define L2DE_TILEDMAP_SMART_COLLIDER_RADIUS "Radius"
#define L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC "IsKinematic"
#define L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER "IsTrigger"
#define L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X "OffsetX"
#define L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y "OffsetY"
#define L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER "MaskLayer"

// Object Group
#define L2DE_TILEDMAP_NODE_OBJECTGROUP "objectgroup"
#define L2DE_TILEDMAP_NODE_OBJECT "object"
#define L2DE_TILEDMAP_NODE_POINT "point"
#define L2DE_TILEDMAP_NODE_ELLIPSE "ellipse"
#define L2DE_TILEDMAP_NODE_POLYGON "polygon"
#define L2DE_TILEDMAP_NODE_TEXT "text"
#define L2DE_TILEDMAP_ATTR_X "x"
#define L2DE_TILEDMAP_ATTR_Y "y"
#define L2DE_TILEDMAP_ATTR_GID "gid"

