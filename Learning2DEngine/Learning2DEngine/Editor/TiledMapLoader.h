#pragma once

#include <vector>
#include <map>
#include <string>

#include <rapidxml/rapidxml.hpp>

#include "TiledMap.h"
#include "TiledMapTileset.h"
#include "../System/Property.h"
#include "../System/GameObject.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class TiledMapLoader final
		{
		private:
			TiledMapLoader() = default;

			static void LoadMapAttributes(TiledMap& map, rapidxml::xml_node<>* mapNode);
			static glm::vec4 ConvertStringToColor(const std::string& hex);

			static std::vector<TiledMapTileset> LoadTilesets(
				rapidxml::xml_node<>* mapNode,
				const std::string& folderPath,
				const std::map<std::string, std::string>& textureMap);

			static bool LoadTileset(
				const std::string& folderPath, 
				const std::string& sourceName,
				const std::map<std::string, std::string>& textureMap,
				TiledMapTileset& tiledMapObject);

			static void LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, std::vector<TiledMapTileset>& tilesets);

			// The folderPath is used when the property type is file.
			static std::map<std::string, System::Property> LoadProperties(rapidxml::xml_node<>* node, const std::string& folderPath = "");

			// The folderPath is used when the property type is file.
			// The sourceName is used for the logging only.
			static std::map<int, std::map<std::string, System::Property>> LoadTilesProperties(
				rapidxml::xml_node<>* node,
				const std::string& sourceName,
				const std::string& folderPath = ""
			);

			// The folderPath is used when the property type is file.
			static std::vector<ObjectItem> LoadObjectItems(rapidxml::xml_node<>* node, const std::string& folderPath = "");

			static bool LoadMapBackground(rapidxml::xml_node<>* mapNode);
			static bool LoadLayerId(rapidxml::xml_node<>* layerNode, int& layerId);

			static void CreateGameObject(TiledMap& map, TiledMapTileset* tileset, int layerId, int imageId, int row, int column);
			static void AddColliderToGameObject(System::GameObject* gameObject, std::map<std::string, System::Property>& properties);
		public:
			~TiledMapLoader() = default;

			static TiledMap LoadFromFile(
				const std::string& filePath,
				const std::map<std::string, std::string>& textureMap = std::map<std::string, std::string>()
			);
		};
	}
}