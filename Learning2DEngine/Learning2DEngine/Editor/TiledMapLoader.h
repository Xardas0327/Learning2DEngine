#pragma once

#include "TiledMap.h"
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
			static void LoadMapAttributes(TiledMap& map, rapidxml::xml_node<>* mapNode);

			static glm::vec4 ConvertBackgroundColor(const std::string& hex);
		public:
			~TiledMapLoader() = default;

			static TiledMap LoadFromFile(const char* filePath);
		};
	}
}