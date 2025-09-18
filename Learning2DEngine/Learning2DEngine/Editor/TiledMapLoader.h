#pragma once

#include <vector>
#include <map>
#include <string>

#include <rapidxml/rapidxml.hpp>

#include "TiledMap.h"
#include "TiledMapObject.h"
#include "../System/Property.h"

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

			static std::vector<TiledMapObject> LoadObjects(
				rapidxml::xml_node<>* mapNode,
				const std::string& folderPath,
				const std::map<std::string, std::string>& textureMap);

			static bool LoadObject(
				const std::string& folderPath, 
				const std::string& sourceName,
				const std::map<std::string, std::string>& textureMap,
				TiledMapObject& tiledMapObject);

			static void LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, std::vector<TiledMapObject>& objects);

			// The folderPath is used when the property type is file.
			static std::map<std::string, System::Property> LoadProperties(rapidxml::xml_node<>* node, const std::string& folderPath = "");
			static std::map<int, std::map<std::string, System::Property>> LoadTilesProperties(
				rapidxml::xml_node<>* node,
				const std::string& sourceName,
				const std::string& folderPath = ""
			);

			static bool LoadMapBackground(rapidxml::xml_node<>* mapNode);
			static bool LoadLayerId(rapidxml::xml_node<>* layerNode, int& layerId);
		public:
			~TiledMapLoader() = default;

			static TiledMap LoadFromFile(
				const std::string& filePath,
				const std::map<std::string, std::string>& textureMap = std::map<std::string, std::string>()
			);
		};
	}
}