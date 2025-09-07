#pragma once

#include <vector>
#include <map>
#include <string>

#include "TiledMap.h"
#include "TiledMapObject.h"
#include <rapidxml/rapidxml.hpp>

namespace Learning2DEngine
{
	namespace Editor
	{
		class TiledMapLoader final
		{
		private:
			TiledMapLoader() = default;

			//If there is valid background color, it will be set by RenderManager::SetClearColor.
			static void LoadMapAttributes(TiledMap& map, rapidxml::xml_node<>* mapNode, bool loadBackground);
			static glm::vec4 ConvertBackgroundColor(const std::string& hex);
			static std::vector<TiledMapObject> LoadObjects(
				rapidxml::xml_node<>* mapNode,
				const std::string& folderPath,
				const std::map<std::string, std::string>& textureMap);
			static bool LoadObject(
				const std::string& folderPath, 
				const std::string& sourceName,
				const std::map<std::string, std::string>& textureMap,
				TiledMapObject& tiledMapObject);
			static void LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, const std::vector<TiledMapObject>& objects);
		public:
			~TiledMapLoader() = default;

			static TiledMap LoadFromFile(
				const std::string& filePath,
				const std::map<std::string, std::string>& textureMap = std::map<std::string, std::string>(),
				bool loadBackground = true);
		};
	}
}