#include "TiledMapLoader.h"

#include <sstream>
#include <exception>
#include <memory>
#include <rapidxml/rapidxml_utils.hpp>

#include "TiledMapConstant.h"
#include "../DebugTool/Log.h"
#include "../Render/RenderManager.h"
#include "../Render/SpriteRenderComponent.h"
#include "../System/ResourceManager.h"
#include "../System/GameObjectManager.h"

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
                auto objects = TiledMapLoader::LoadObjects(mapNode, folderPath);
                LoadLayers(map, mapNode, objects);

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
                catch (const std::exception&)
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
            catch (const std::exception&)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the background color - RGB is not valid: " + hex);
                return color;
            }

            return color;
        }

        std::vector<TiledMapObject> TiledMapLoader::LoadObjects(rapidxml::xml_node<>* mapNode, const std::string& folderPath)
        {
            std::vector<TiledMapObject> objects;
            for (
                auto mapTileset = mapNode->first_node(L2DE_TILEDMAP_NODE_TILESET);
                mapTileset != nullptr;
                mapTileset = mapTileset->next_sibling(L2DE_TILEDMAP_NODE_TILESET)
                )
            {
                TiledMapObject object;
                object.firstGid = std::atoi(mapTileset->first_attribute(L2DE_TILEDMAP_ATTR_FIRSTGID)->value());
                if (object.firstGid <= 0)
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

                if (TiledMapLoader::LoadObject(folderPath, sourceName, object))
                    objects.push_back(object);
            }
            return objects;
        }

        bool TiledMapLoader::LoadObject(const std::string& folderPath, const std::string& sourceName, TiledMapObject& tiledMapObject)
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

            tiledMapObject.size.x = static_cast<float>(std::atoi(tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEWIDTH)->value()));
            if (tiledMapObject.size.x <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile width should be bigger then 0.");
            }
            tiledMapObject.size.y = static_cast<float>(std::atoi(tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEHEIGHT)->value()));
            if (tiledMapObject.size.y <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile height should be bigger then 0.");
            }

            tiledMapObject.columns = std::atoi(tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_COLUMNS)->value());
            if (tiledMapObject.columns <= 0)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset columns should be bigger then 0.");
            }

            tiledMapObject.rows = tiledMapObject.columns == 0 ? 0 :
                std::atoi(tilesetNode->first_attribute(L2DE_TILEDMAP_ATTR_TILECOUNT)->value()) / tiledMapObject.columns;
            if (tiledMapObject.rows <= 0)
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
                tiledMapObject.texture = &resourceManager.GetTexture(name);
            }
            else
            {
                tiledMapObject.texture = &resourceManager.LoadTextureFromFile(
                    name,
                    (folderPath + imageSource).c_str(),
                    Texture2DSettings(true));
            }

            return true;
        }

        void TiledMapLoader::LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, const std::vector<TiledMapObject>& objects)
        {
            auto& gameObjectManager = System::GameObjectManager::GetInstance();
            int layerId = 0;
            for (
                auto layerNode = mapNode->first_node(L2DE_TILEDMAP_NODE_LAYER);
                layerNode != nullptr;
                layerNode = layerNode->next_sibling(L2DE_TILEDMAP_NODE_LAYER)
                )
            {
                int layerWidth = std::atoi(layerNode->first_attribute(L2DE_TILEDMAP_ATTR_WIDTH)->value());
                if (layerWidth != map.GetWidth())
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the layer width is not equal to the map width.");
                }

                int layerHeight = std::atoi(layerNode->first_attribute(L2DE_TILEDMAP_ATTR_HEIGHT)->value());
                if (layerHeight != map.GetHeight())
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the layer height is not equal to the map height.");
                }

                auto dataNode = layerNode->first_node(L2DE_TILEDMAP_NODE_DATA);
                if (dataNode == nullptr)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the layer data node is missing.");
                    continue;
                }

                auto encodingAttr = dataNode->first_attribute(L2DE_TILEDMAP_ATTR_ENCODING);
                if (encodingAttr == nullptr)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the layer data encoding attribute is missing.");
                    continue;
                }

                std::string encoding = encodingAttr->value();
                if (encoding != L2DE_TILEDMAP_SUPPORTED_ENCODING)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the layer data encoding is not supported: "
                        + encoding + "\n Supported encoding: " + L2DE_TILEDMAP_SUPPORTED_ENCODING);
                    continue;
                }

                std::stringstream dataText(dataNode->value());
                std::string line;

				int row = 0;
                while (std::getline(dataText, line)) {
                    //Remove \r
                    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
                    if(line.empty())
						continue;

                    std::stringstream lineStream(line);
                    std::string token;

					int column = 0;
                    while (std::getline(lineStream, token, ',')) {
                        if (!token.empty()) {
							int id = std::atoi(token.c_str());
                            if (id > 0)
                            {
                                const TiledMapObject* selectedObject = nullptr;
                                for (auto& object : objects)
                                {
                                    if (object.HasNumber(id))
                                    {
                                        selectedObject = &object;
                                        break;
                                    }
                                }

                                if (selectedObject == nullptr)
                                {
                                    L2DE_LOG_WARNING("TiledMapLoader: the tile id " + std::to_string(id) + " is not valid.");
                                    continue;
                                }

                                Transform transform(
                                    glm::vec2(
                                        static_cast<float>(column) * map.GetTileWidth(),
                                        static_cast<float>(row + 1) * map.GetTileHeight() - selectedObject->size.y),
                                    selectedObject->size
                                );
                                auto gameObject = gameObjectManager.CreateGameObject(transform);
                                auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
                                    RendererMode::RENDER,
                                    *selectedObject->texture,
                                    layerId);
                                renderer->data.uvMatrix = selectedObject->GetUV(id);
                            }
                            
                        }
                        ++column;
                    }
                    ++row;
                }
                ++layerId;
            }
        }
    }
}