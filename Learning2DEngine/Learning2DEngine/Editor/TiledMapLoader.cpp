#include "TiledMapLoader.h"

#include <string>
#include <exception>
#include <rapidxml/rapidxml_utils.hpp>

#include "TiledMapConstant.h"
#include "../DebugTool/Log.h"
#include "../Render/RenderManager.h"

namespace Learning2DEngine
{
    using namespace Render;

    namespace Editor
    {
        TiledMap TiledMapLoader::LoadFromFile(const std::string& filePath)
        {
            auto doc = new rapidxml::xml_document<>;
            try
            {
                std::size_t folderIndex = filePath.find_last_of("/");
                std::string folderPath = folderIndex == std::string::npos? "" : filePath.substr(0, folderIndex);

                rapidxml::file<> xmlFile(filePath.c_str());
                doc->parse<0>(xmlFile.data());
                TiledMap map;

                // Map node
                auto mapNode = doc->first_node(L2DE_TILEDMAP_NODE_MAP);
                if (mapNode == nullptr)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the map node is missing.");
                    return map;
                }

                TiledMapLoader::LoadMapAttributes(map, mapNode);

                delete doc;
                return map;
            }
            catch (std::exception& e)
            {
                delete doc;
                L2DE_LOG_ERROR((std::string)"TiledMapLoader: " + e.what());
                throw e;
            }
        }
        void TiledMapLoader::LoadMapAttributes(TiledMap& map, rapidxml::xml_node<>* mapNode)
        {
            map.version = mapNode->first_attribute(L2DE_TILEDMAP_ATTR_VERSION)->value();
            if (map.version != L2DE_TILEDMAP_SUPPORTED_VERSION)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the map version is not supported: "
                    + map.version + "\n Supported version: " + L2DE_TILEDMAP_SUPPORTED_VERSION);
            }

            map.tiledVersion = mapNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEDVERSION)->value();

            map.orientation = mapNode->first_attribute(L2DE_TILEDMAP_ATTR_ORIENTATION)->value();
            if (map.orientation != L2DE_TILEDMAP_SUPPORTED_ORIENTATION)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the map orientation is not supported: "
                    + map.orientation + "\n Supported version: " + L2DE_TILEDMAP_SUPPORTED_ORIENTATION);
            }

            map.width = std::atoi(mapNode->first_attribute(L2DE_TILEDMAP_ATTR_WIDTH)->value());
            if (map.width <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the map width should be bigger then 0: " + std::to_string(map.width));
            }

            map.height = std::atoi(mapNode->first_attribute(L2DE_TILEDMAP_ATTR_HEIGHT)->value());
            if (map.height <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the map height should be bigger then 0: " + std::to_string(map.height));
            }

            map.tileWidth = std::atoi(mapNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEWIDTH)->value());
            if (map.tileWidth <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the map tile width should be bigger then 0: " + std::to_string(map.tileWidth));
            }

            map.tileHeight = std::atoi(mapNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEHEIGHT)->value());
            if (map.tileHeight <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the map tile height should be bigger then 0: " + std::to_string(map.tileHeight));
            }

            map.infinite = mapNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEHEIGHT)->value() == "1" ? true : false;
            if (map.infinite)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the map is infinite. This is not supported.");
            }

            auto backgroundColorAttr = mapNode->first_attribute(L2DE_TILEDMAP_ATTR_BACKGROUND_COLOR);
            if (backgroundColorAttr != nullptr)
            {
                map.backgroundColor = TiledMapLoader::ConvertBackgroundColor(backgroundColorAttr->value());
                RenderManager::GetInstance().SetClearColor(
                    map.backgroundColor.r,
                    map.backgroundColor.g,
                    map.backgroundColor.b,
                    map.backgroundColor.a);
            }
        }

        glm::vec4 TiledMapLoader::ConvertBackgroundColor(const std::string& hex)
        {
            glm::vec4 color(1.0f);

            if (hex.empty())
                return color;

            if (hex.length() != 7 && hex.length() != 9)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the background color is not valid: " + hex);
                return color;
            }

            int shift = 0;
            if (hex.length() == 9)
            {
                shift = 2;
                try
                {
                    int alpha = std::stoi(hex.substr(0, 2), nullptr, 16);
                    color.a = static_cast<float>(alpha) / 255.0f;
                }
                catch (std::exception& e)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the background color - alpha is not valid: " + hex);
                    return color;
                }
            }

            try
            {
                int r = std::stoi(hex.substr(1 + shift, 2), nullptr, 16);
                color.r = static_cast<float>(r) / 255.0f;

                int g = std::stoi(hex.substr(3 + shift, 2), nullptr, 16);
                color.g = static_cast<float>(g) / 255.0f;

                int b = std::stoi(hex.substr(5 + shift, 2), nullptr, 16);
                color.b = static_cast<float>(b) / 255.0f;
            }
            catch (std::exception& e)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the background color - RGB is not valid: " + hex);
                return color;
            }

            return color;
        }
    }
}