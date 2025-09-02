#include "TiledMapLoader.h"

#include <exception>
#include <memory>
#include <rapidxml/rapidxml_utils.hpp>

#include "TiledMapConstant.h"
#include "../DebugTool/Log.h"
#include "../Render/RenderManager.h"
#include "../System/ResourceManager.h"

namespace Learning2DEngine
{
    using namespace Render;
    using namespace System;

    namespace Editor
    {
        TiledMap TiledMapLoader::LoadFromFile(const std::string& filePath)
        {
            auto doc = std::make_unique<rapidxml::xml_document<>>();
            try
            {
                std::size_t folderIndex = filePath.find_last_of("/");
                std::string folderPath = folderIndex == std::string::npos ? "" : filePath.substr(0, folderIndex) + "/";

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
                auto textures = TiledMapLoader::LoadTextures(mapNode, folderPath);

                return map;
            }
            catch (std::exception& e)
            {
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

        std::vector<TiledMapTexture> TiledMapLoader::LoadTextures(rapidxml::xml_node<>* mapNode, const std::string& folderPath)
        {
            std::vector<TiledMapTexture> textures;
            Texture2DSettings textureSettings(true);
            for (
                auto mapTileset = mapNode->first_node(L2DE_TILEDMAP_NODE_TILESET);
                mapTileset != nullptr;
                mapTileset = mapTileset->next_sibling(L2DE_TILEDMAP_NODE_TILESET)
                )
            {
                TiledMapTexture texture;
                texture.firstGid = std::atoi(mapTileset->first_attribute(L2DE_TILEDMAP_ATTR_FIRSTGID)->value());
                if (texture.firstGid <= 0)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the tileset firstgid should be bigger then 0.");
                    continue;
                }

                std::string sourceName = mapTileset->first_attribute(L2DE_TILEDMAP_ATTR_SOURCE)->value();
                if (sourceName.empty())
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the tileset source is empty.");
                    continue;
                }

                if (TiledMapLoader::LoadTexture(folderPath, sourceName, texture))
                    textures.push_back(texture);
            }
            return textures;
        }

        bool TiledMapLoader::LoadTexture(const std::string& folderPath, const std::string& sourceName, TiledMapTexture& tiledMapTexture)
        {
            rapidxml::file<> xmlFile((folderPath + sourceName).c_str());
            auto doc = std::make_unique<rapidxml::xml_document<>>();
            doc->parse<0>(xmlFile.data());

            auto tilesetNode = doc->first_node(L2DE_TILEDMAP_NODE_TILESET);

            std::string version = tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_VERSION)->value();
            if (version != L2DE_TILEDMAP_SUPPORTED_VERSION)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the " + sourceName + " tileset version is not supported: "
                    + version + "\n Supported version: " + L2DE_TILEDMAP_SUPPORTED_VERSION);
            }

            tiledMapTexture.objectSize.x = std::atoi(tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEWIDTH)->value());
            if (tiledMapTexture.objectSize.x <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile width should be bigger then 0.");
            }
            tiledMapTexture.objectSize.y = std::atoi(tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEHEIGHT)->value());
            if (tiledMapTexture.objectSize.y <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile height should be bigger then 0.");
            }

            tiledMapTexture.columns = std::atoi(tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_COLUMNS)->value());
            if (tiledMapTexture.columns <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset columns should be bigger then 0.");
            }

            tiledMapTexture.rows = tiledMapTexture.columns == 0 ? 0 :
                std::atoi(tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_TILECOUNT)->value()) / tiledMapTexture.columns;
            if (tiledMapTexture.rows <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset rows should be bigger then 0.");
            }

            std::string name = tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_NAME)->value();
            if (name.empty())
            {
                L2DE_LOG_ERROR("TiledMapLoader: the tileset name is empty.");
                return false;
            }

            std::string imageSource = tilesetNode->first_node(L2DE_TILEDMAP_NODE_IMAGE)->first_attribute(L2DE_TILEDMAP_ATTR_SOURCE)->value();
            if (imageSource.empty())
            {
                L2DE_LOG_ERROR("TiledMapLoader: the tileset image source is empty.");
                return false;
            }

            auto& resourceManager = ResourceManager::GetInstance();
            if (resourceManager.IsTextureExist(name))
            {
                tiledMapTexture.texture = &resourceManager.GetTexture(name);
            }
            else
            {
                tiledMapTexture.texture = &resourceManager.LoadTextureFromFile(
                    name,
                    (folderPath + imageSource).c_str(),
                    Texture2DSettings(true));
            }

            return true;
        }
    }
}