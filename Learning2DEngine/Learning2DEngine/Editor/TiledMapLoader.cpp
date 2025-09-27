#include "TiledMapLoader.h"

#include <sstream>
#include <exception>
#include <memory>
#include <GLFW/glfw3.h>
#include <rapidxml/rapidxml_utils.hpp>

#include "../DebugTool/DebugMacro.h"
#include "../DebugTool/Log.h"
#include "../Render/RenderManager.h"
#include "../Render/SpriteRenderComponent.h"
#include "../System/ResourceManager.h"
#include "../Physics/BoxColliderComponent.h"
#include "../Physics/CircleColliderComponent.h"


namespace Learning2DEngine
{
    using namespace Render;
    using namespace System;
    using namespace Physics;

    namespace Editor
    {
        TiledMap TiledMapLoader::LoadFromFile(const std::string& filePath,
            const std::map<std::string, std::string>& textureMap)
        {
#if L2DE_DEBUG
            float startTime = static_cast<float>(glfwGetTime());
#endif
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
                auto tilesets = TiledMapLoader::LoadTilesets(mapNode, folderPath, textureMap);
                TiledMapLoader::LoadLayers(map, mapNode, tilesets);
                TiledMapLoader::LoadObjectLayers(map, mapNode, tilesets, folderPath);

#if L2DE_DEBUG
                float loadingTime = static_cast<float>(glfwGetTime()) - startTime;
                L2DE_LOG_INFO("TiledMapLoader: " + filePath + " file's loading time: " + std::to_string(loadingTime) + "s");
#endif

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
            bool foundWidth = false;
            bool foundHeight = false;
            bool foundTileWidth = false;
            bool foundTileHeight = false;
            for (auto attr = mapNode->first_attribute();
                attr != nullptr;
                attr = attr->next_attribute())
            {
                if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_VERSION) == 0)
                {
                    map.version = attr->value();
                    if (map.version != L2DE_TILEDMAP_SUPPORTED_VERSION)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the map version is not supported: "
                            + map.version + "\n Supported version: " + L2DE_TILEDMAP_SUPPORTED_VERSION);
                    }
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_TILEDVERSION) == 0)
                {
                    map.tiledVersion = attr->value();
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_ORIENTATION) == 0)
                {
                    map.orientation = attr->value();
                    if (map.orientation != L2DE_TILEDMAP_SUPPORTED_ORIENTATION)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the map orientation is not supported: "
                            + map.orientation + "\n Supported version: " + L2DE_TILEDMAP_SUPPORTED_ORIENTATION);
                    }
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_WIDTH) == 0)
                {
                    map.width = std::atoi(attr->value());
                    foundWidth = true;

                    if (map.width <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the map width should be bigger then 0: " + std::to_string(map.width));
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_HEIGHT) == 0)
                {
                    map.height = std::atoi(attr->value());
                    foundHeight = true;

                    if (map.height <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the map height should be bigger then 0: " + std::to_string(map.height));
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_TILEWIDTH) == 0)
                {
                    map.tileWidth = std::atoi(attr->value());
                    foundTileWidth = true;
                    if (map.tileWidth <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the map tile width should be bigger then 0: " + std::to_string(map.tileWidth));
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_TILEHEIGHT) == 0)
                {
                    map.tileHeight = std::atoi(attr->value());
                    foundTileHeight = true;
                    if (map.tileHeight <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the map tile height should be bigger then 0: " + std::to_string(map.tileWidth));
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_INFINITE) == 0)
                {
                    map.infinite = strcmp(attr->value(), "1") == 0 ? true : false;
                    if (map.infinite)
                        L2DE_LOG_WARNING("TiledMapLoader: the map is infinite. This is not supported.");
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_BACKGROUND_COLOR) == 0)
                {
                    map.backgroundColor = TiledMapLoader::ConvertStringToColor(attr->value());
                }
            }

            if (!foundWidth)
                L2DE_LOG_ERROR("TiledMapLoader: the map width is missing.");
            if (!foundHeight)
                L2DE_LOG_ERROR("TiledMapLoader: the map height is missing.");
            if (!foundTileWidth)
                L2DE_LOG_ERROR("TiledMapLoader: the map tile width is missing.");
            if (!foundTileHeight)
                L2DE_LOG_ERROR("TiledMapLoader: the map tile height is missing.");

            if (TiledMapLoader::LoadMapBackground(mapNode))
            {
                RenderManager::GetInstance().SetClearColor(
                    map.backgroundColor.r,
                    map.backgroundColor.g,
                    map.backgroundColor.b,
                    map.backgroundColor.a);
            }
        }

        glm::vec4 TiledMapLoader::ConvertStringToColor(const std::string& hex)
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
                    int alpha = std::stoi(hex.substr(1, 2), nullptr, 16);
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

        std::vector<TiledMapTileset> TiledMapLoader::LoadTilesets(
            rapidxml::xml_node<>* mapNode,
            const std::string& folderPath,
            const std::map<std::string, std::string>& textureMap)
        {
            std::vector<TiledMapTileset> tilesets;
            for (
                auto mapTileset = mapNode->first_node(L2DE_TILEDMAP_NODE_TILESET);
                mapTileset != nullptr;
                mapTileset = mapTileset->next_sibling(L2DE_TILEDMAP_NODE_TILESET)
                )
            {
                TiledMapTileset tileset;
                tileset.firstGid = std::atoi(mapTileset->first_attribute(L2DE_TILEDMAP_ATTR_FIRSTGID)->value());
                if (tileset.firstGid <= 0)
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

                if (TiledMapLoader::LoadTileset(folderPath, sourceName, textureMap, tileset))
                    tilesets.push_back(tileset);
            }
            return tilesets;
        }

        bool TiledMapLoader::LoadTileset(
            const std::string& folderPath,
            const std::string& sourceName,
            const std::map<std::string, std::string>& textureMap,
            TiledMapTileset& tiledMapObject)
        {
            rapidxml::file<> xmlFile((folderPath + sourceName).c_str());
            auto doc = std::make_unique<rapidxml::xml_document<>>();
            doc->parse<0>(xmlFile.data());

            auto tilesetNode = doc->first_node(L2DE_TILEDMAP_NODE_TILESET);
            if (tilesetNode == nullptr)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset node is missing.");
                return false;
            }

            bool foundVersion = false;
            bool foundTileWidth = false;
            bool foundTileHeight = false;
            bool foundColumns = false;
            bool foundTileCount = false;
            std::string imageName = "";

            for (auto attr = tilesetNode->first_attribute();
                attr != nullptr;
                attr = attr->next_attribute())
            {
                if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_VERSION) == 0)
                {
                    std::string version = attr->value();
                    foundVersion = true;

                    if (version != L2DE_TILEDMAP_SUPPORTED_VERSION)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the " + sourceName + " tileset version is not supported: "
                            + version + "\n Supported version: " + L2DE_TILEDMAP_SUPPORTED_VERSION);
                    }
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_TILEWIDTH) == 0)
                {
                    tiledMapObject.tiledSize.x = static_cast<float>(std::atoi(attr->value()));
                    foundTileWidth = true;

                    if (tiledMapObject.tiledSize.x <= 0.0f)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile width should be bigger then 0.");
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_TILEHEIGHT) == 0)
                {
                    tiledMapObject.tiledSize.y = static_cast<float>(std::atoi(attr->value()));
                    foundTileHeight = true;

                    if (tiledMapObject.tiledSize.y <= 0.0f)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile height should be bigger then 0.");
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_COLUMNS) == 0)
                {
                    tiledMapObject.columns = std::atoi(attr->value());
                    foundColumns = true;

                    if (tiledMapObject.columns <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset columns should be bigger then 0.");
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_TILECOUNT) == 0)
                {
                    tiledMapObject.tileCount = std::atoi(attr->value());
                    foundTileCount = true;

                    if (tiledMapObject.tileCount <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tilecount should be bigger then 0.");
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_NAME) == 0)
                {
                    imageName = attr->value();
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_SPACING) == 0)
                {
                    tiledMapObject.spacing = std::atoi(attr->value());
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_MARGIN) == 0)
                {
                    tiledMapObject.margin = std::atoi(attr->value());
                }
            }

            if (!foundVersion)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset version is missing.");
            if (!foundTileWidth)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile width is missing.");
            if (!foundTileHeight)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile height is missing.");
            if (!foundColumns)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset columns is missing.");
            if (!foundTileCount)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tilecount is missing.");

            if (imageName.empty())
            {
                L2DE_LOG_ERROR("TiledMapLoader: the tileset name is empty.");
                return false;
            }

            auto imageNode = tilesetNode->first_node(L2DE_TILEDMAP_NODE_IMAGE);
            if (imageNode == nullptr)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset image node is missing.");
                return false;
            }

            bool foundWidth = false;
            bool foundHeight = false;
            std::string imageSource = "";

            for (auto attr = imageNode->first_attribute();
                attr != nullptr;
                attr = attr->next_attribute())
            {
                if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_SOURCE) == 0)
                {
                    imageSource = attr->value();
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_WIDTH) == 0)
                {
                    tiledMapObject.imageSize.x = static_cast<float>(std::atoi(attr->value()));
                    foundWidth = true;

                    if (tiledMapObject.imageSize.x <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " image width should be bigger then 0.");
                }
                else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_HEIGHT) == 0)
                {
                    tiledMapObject.imageSize.y = static_cast<float>(std::atoi(attr->value()));
                    foundHeight = true;

                    if (tiledMapObject.imageSize.y <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " image height should be bigger then 0.");
                }
            }
            if (!foundWidth)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " image width is missing.");
            if (!foundHeight)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " image height is missing.");

            auto& resourceManager = ResourceManager::GetInstance();
            if (textureMap.find(imageName) != textureMap.end())
            {
                tiledMapObject.texture = &resourceManager.GetTexture(textureMap.at(imageName));
            }
            else if (resourceManager.IsTextureExist(imageName))
            {
                tiledMapObject.texture = &resourceManager.GetTexture(imageName);
            }
            else
            {

                if (imageSource.empty())
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the tileset image source is empty.");
                    return false;
                }

                tiledMapObject.texture = &resourceManager.LoadTextureFromFile(
                    imageName,
                    (folderPath + imageSource).c_str(),
                    Texture2DSettings(true));
            }

            tiledMapObject.commonProperties = TiledMapLoader::LoadProperties(tilesetNode, folderPath);
            tiledMapObject.uniqueProperties = TiledMapLoader::LoadTilesProperties(tilesetNode, sourceName, folderPath);
            tiledMapObject.objects = TiledMapLoader::LoadTilesObjectItems(tilesetNode, sourceName, folderPath);

            return true;
        }

        void TiledMapLoader::LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, std::vector<TiledMapTileset>& tilesets)
        {
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

                int overrideLayerId = 0;
                bool useOverrideLayerId = TiledMapLoader::LoadLayerId(layerNode, overrideLayerId);

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
                    if (line.empty())
                        continue;

                    std::stringstream lineStream(line);
                    std::string token;

                    int column = 0;
                    while (std::getline(lineStream, token, ',')) {
                        if (!token.empty()) {
                            int id = std::atoi(token.c_str());
                            if (id > 0)
                            {
                                TiledMapTileset* selectedTileset = nullptr;
                                for (auto& tileset : tilesets)
                                {
                                    if (tileset.HasNumber(id))
                                    {
                                        selectedTileset = &tileset;
                                        break;
                                    }
                                }

                                if (selectedTileset == nullptr)
                                {
                                    L2DE_LOG_WARNING("TiledMapLoader: the tile id " + std::to_string(id) + " is not valid.");
                                    continue;
                                }

                                TiledMapLoader::CreateGameObjectFromLayerData(
                                    map,
                                    selectedTileset,
                                    useOverrideLayerId ? overrideLayerId : layerId,
                                    id,
                                    row,
                                    column);
                            }

                        }
                        ++column;
                    }
                    ++row;
                }
                ++layerId;
            }
        }

        void TiledMapLoader::LoadObjectLayers(
            TiledMap& map,
            rapidxml::xml_node<>* mapNode,
            std::vector<TiledMapTileset>& tilesets,
            const std::string& folderPath
        )
        {
            int layerId = 0;
            for (
                auto objectLayerNode = mapNode->first_node(L2DE_TILEDMAP_NODE_OBJECTGROUP);
                objectLayerNode != nullptr;
                objectLayerNode = objectLayerNode->next_sibling(L2DE_TILEDMAP_NODE_OBJECTGROUP)
                )
            {
                int overrideLayerId = 0;
                bool useOverrideLayerId = TiledMapLoader::LoadLayerId(objectLayerNode, overrideLayerId);

				auto objects = LoadObjectItems(objectLayerNode, folderPath);
                for (auto& object : objects)
                {
                    CreateGameObjectFromObjectLayerData(
                        map,
                        object,
                        tilesets,
						useOverrideLayerId ? overrideLayerId : layerId
                    );
                }

                ++layerId;
            }
        }

        std::map<std::string, System::Property> TiledMapLoader::LoadProperties(rapidxml::xml_node<>* node, const std::string& folderPath)
        {
            std::map<std::string, Property> properties;
            if (node == nullptr)
                return properties;
            if (strcmp(node->name(), L2DE_TILEDMAP_NODE_PROPERTIES) != 0)
            {
                node = node->first_node(L2DE_TILEDMAP_NODE_PROPERTIES);
            }

            if (node == nullptr)
                return properties;

            for (
                auto property = node->first_node(L2DE_TILEDMAP_NODE_PROPERTY);
                property != nullptr;
                property = property->next_sibling(L2DE_TILEDMAP_NODE_PROPERTY)
                )
            {
                auto nameAttr = property->first_attribute(L2DE_TILEDMAP_ATTR_NAME);
                if (nameAttr == nullptr)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the property name attribute is missing.");
                    continue;
                }

                auto valueAttr = property->first_attribute(L2DE_TILEDMAP_ATTR_VALUE);
                if (valueAttr == nullptr)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the property value attribute is missing.");
                    continue;
                }

                auto typeAttr = property->first_attribute(L2DE_TILEDMAP_ATTR_TYPE);
                PropertyType type = PropertyType::String;
                if (typeAttr != nullptr)
                {
                    std::string typeStr = typeAttr->value();
                    if (typeStr == L2DE_TILEDMAP_PROPERTY_TYPE_BOOL)
                        type = PropertyType::Bool;
                    else if (typeStr == L2DE_TILEDMAP_PROPERTY_TYPE_COLOR)
                        type = PropertyType::Color;
                    else if (typeStr == L2DE_TILEDMAP_PROPERTY_TYPE_FILE)
                        type = PropertyType::File;
                    else if (typeStr == L2DE_TILEDMAP_PROPERTY_TYPE_FLOAT)
                        type = PropertyType::Float;
                    else if (typeStr == L2DE_TILEDMAP_PROPERTY_TYPE_INT)
                        type = PropertyType::Int;
                    else if (typeStr == L2DE_TILEDMAP_PROPERTY_TYPE_STRING)
                        type = PropertyType::String;
                    else
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the property type is not valid: " + typeStr);
                        continue;
                    }
                }

                switch (type)
                {
                case PropertyType::Bool:
                    properties.emplace(nameAttr->value(), Property(strcmp(valueAttr->value(), "true") == 0 ? true : false));
                    break;
                case PropertyType::Color:
                    properties.emplace(nameAttr->value(), Property(TiledMapLoader::ConvertStringToColor(valueAttr->value())));
                    break;
                case PropertyType::File:
                    properties.emplace(nameAttr->value(), Property(folderPath + valueAttr->value(), type));
                    break;
                case PropertyType::Float:
                    properties.emplace(nameAttr->value(), Property(static_cast<float>(std::atof(valueAttr->value()))));
                    break;
                case PropertyType::Int:
                    properties.emplace(nameAttr->value(), Property(std::atoi(valueAttr->value())));
                    break;
                case PropertyType::String:
                    properties.emplace(nameAttr->value(), Property(valueAttr->value(), type));
                    break;
                default:
                    break;
                }
            }

            return properties;
        }

        std::map<int, std::map<std::string, System::Property>> TiledMapLoader::LoadTilesProperties(
            rapidxml::xml_node<>* node,
            const std::string& sourceName,
            const std::string& folderPath
        )
        {
            std::map<int, std::map<std::string, System::Property>> properties;

            for (
                auto tile = node->first_node(L2DE_TILEDMAP_NODE_TILE);
                tile != nullptr;
                tile = tile->next_sibling(L2DE_TILEDMAP_NODE_TILE)
                )
            {
                auto idAttr = tile->first_attribute(L2DE_TILEDMAP_ATTR_ID);
                if (idAttr == nullptr)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: " + sourceName + " the tile id attribute is missing.");
                    continue;
                }

                int id = std::atoi(idAttr->value());
                if (properties.count(id))
                {
                    L2DE_LOG_WARNING("TiledMapLoader: " + sourceName + " the tile id is duplicated: " + std::to_string(id));
                    continue;
                }

                auto tileProperties = LoadProperties(tile, folderPath);
                if (!tileProperties.empty())
                    properties.emplace(id, std::move(tileProperties));
            }

            return properties;
        }

        std::map<int, std::vector<ObjectItem>> TiledMapLoader::LoadTilesObjectItems(
            rapidxml::xml_node<>* node,
            const std::string& sourceName,
            const std::string& folderPath
        )
        {
            std::map<int, std::vector<ObjectItem>> objects;

            for (
                auto tile = node->first_node(L2DE_TILEDMAP_NODE_TILE);
                tile != nullptr;
                tile = tile->next_sibling(L2DE_TILEDMAP_NODE_TILE)
                )
            {
                auto idAttr = tile->first_attribute(L2DE_TILEDMAP_ATTR_ID);
                if (idAttr == nullptr)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: " + sourceName + " the tile id attribute is missing.");
                    continue;
                }

                int id = std::atoi(idAttr->value());
                if (objects.count(id))
                {
                    L2DE_LOG_WARNING("TiledMapLoader: " + sourceName + " the tile id is duplicated: " + std::to_string(id));
                    continue;
                }

                auto tileObjects = LoadObjectItems(tile, folderPath);
                if (!tileObjects.empty())
                    objects.emplace(id, std::move(tileObjects));
            }

            return objects;
        }

        std::vector<ObjectItem> TiledMapLoader::LoadObjectItems(rapidxml::xml_node<>* node, const std::string& folderPath)
        {
            std::vector<ObjectItem> objects;
            if (node == nullptr)
                return objects;

            if (strcmp(node->name(), L2DE_TILEDMAP_NODE_OBJECTGROUP) != 0)
            {
                node = node->first_node(L2DE_TILEDMAP_NODE_OBJECTGROUP);
            }

            if (node == nullptr)
                return objects;

            for (
                auto object = node->first_node(L2DE_TILEDMAP_NODE_OBJECT);
                object != nullptr;
                object = object->next_sibling(L2DE_TILEDMAP_NODE_OBJECT)
                )
            {
                auto child = object->first_node();

                //the properties will be check later
                if (child != nullptr && strcmp(child->name(), L2DE_TILEDMAP_NODE_PROPERTIES) == 0)
                    child = child->next_sibling();

                ObjectType type = ObjectType::Box;
                if (child == nullptr)
                {
                    //it should be box.
                }
                else if (strcmp(child->name(), L2DE_TILEDMAP_NODE_POLYGON) == 0)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: polygon object type is not supported.");
                    continue;
                }
                else if (strcmp(child->name(), L2DE_TILEDMAP_NODE_TEXT) == 0)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: text object type is not supported.");
                    continue;
                }
                else if (strcmp(child->name(), L2DE_TILEDMAP_NODE_POINT) == 0)
                {
                    type = ObjectType::Point;
                }
                else if (strcmp(child->name(), L2DE_TILEDMAP_NODE_ELLIPSE) == 0)
                {
                    type = ObjectType::Ellipse;
                }

                bool foundGid = false;
                bool foundX = false;
                bool foundY = false;
                bool foundWidth = false;
                bool foundHeight = false;
                int gid;
                glm::vec2 position;
                glm::vec2 size;
                for (auto attr = object->first_attribute();
                    attr != nullptr;
                    attr = attr->next_attribute())
                {
                    if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_X) == 0)
                    {
                        position.x = static_cast<float>(std::atof(attr->value()));
                        foundX = true;
                    }
                    else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_Y) == 0)
                    {
                        position.y = static_cast<float>(std::atof(attr->value()));
                        foundY = true;
                    }
                    else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_WIDTH) == 0)
                    {
                        size.x = static_cast<float>(std::atof(attr->value()));
                        foundWidth = true;

                        if (size.x <= 0)
                            L2DE_LOG_ERROR("TiledMapLoader: an object width should be bigger then 0: " + std::to_string(size.x));
                    }
                    else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_HEIGHT) == 0)
                    {
                        size.y = static_cast<float>(std::atof(attr->value()));
                        foundHeight = true;

                        if (size.y <= 0)
                            L2DE_LOG_ERROR("TiledMapLoader: an object height should be bigger then 0: " + std::to_string(size.y));
                    }
                    else if (strcmp(attr->name(), L2DE_TILEDMAP_ATTR_GID) == 0)
                    {
                        gid = std::atoi(attr->value());
                        type = ObjectType::Image;
                        if (gid <= 0)
                            L2DE_LOG_ERROR("TiledMapLoader: an object gid should be bigger then 0: " + std::to_string(gid));
                        else
                            foundGid = true;
                    }
                }

                auto properties = TiledMapLoader::LoadProperties(object, folderPath);

                switch (type)
                {
                case ObjectType::Point:
                    if (!foundX || !foundY)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: a point object can't be created, because some data is missing.");
                        continue;
                    }
                    objects.push_back(ObjectItem(std::move(
                        ObjectPoint(position, std::move(properties))
                    )));
                    break;
                case ObjectType::Box:
                    if (!foundX || !foundY || !foundWidth || !foundHeight)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: a box object can't be created, because some data is missing.");
                        continue;
                    }
                    objects.push_back(ObjectItem(std::move(
                        ObjectBox(position, size, std::move(properties))
                    )));
                    break;
                case ObjectType::Ellipse:
                    if (!foundX || !foundY || !foundWidth || !foundHeight)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: a ellipse object can't be created, because some data is missing.");
                        continue;
                    }
                    //We want radius instead of diameter.
					size /= 2.0f;
                    //At the moment, there is circle, not ellipse collider.
                    if (size.x != size.y)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: an ellipse object should have the same width and height. Only the width will be used.");
                        size.y = size.x;
                    }
                    objects.push_back(ObjectItem(std::move(
                        ObjectEllipse(position, size, std::move(properties))
                    )));
                    break;
                case ObjectType::Image:
                    if (!foundX || !foundY || !foundWidth || !foundHeight || !foundGid)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: a ellipse object can't be created, because some data is missing.");
                        continue;
                    }
                    objects.push_back(ObjectItem(std::move(
                        ObjectImage(position, size, std::move(properties), gid)
                    )));
                    break;
                }

            }
            return objects;
        }

        bool TiledMapLoader::LoadMapBackground(rapidxml::xml_node<>* mapNode)
        {

            auto properties = TiledMapLoader::LoadProperties(mapNode);
#if L2DE_DEBUG
            int tooMuchProperties = 1;
#endif

            if (properties.count(L2DE_TILEDMAP_SMART_LOADBACKGROUND))
            {
#if L2DE_DEBUG
                ++tooMuchProperties;
#endif
                if (properties[L2DE_TILEDMAP_SMART_LOADBACKGROUND].GetType() == PropertyType::Bool)
                {
                    return properties[L2DE_TILEDMAP_SMART_LOADBACKGROUND].GetBool();
                }
                else
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the " L2DE_TILEDMAP_SMART_LOADBACKGROUND " property type is not valid. It should be Bool.");
                }
            }

#if L2DE_DEBUG
            if (properties.size() >= tooMuchProperties)
            {
                L2DE_LOG_WARNING("TiledMapLoader: The Map's properties won't be processed. Except: " L2DE_TILEDMAP_SMART_LOADBACKGROUND);
            }
#endif
            return false;
        }

        bool TiledMapLoader::LoadLayerId(rapidxml::xml_node<>* layerNode, int& layerId)
        {
            auto properties = TiledMapLoader::LoadProperties(layerNode);
#if L2DE_DEBUG
            int tooMuchProperties = 1;
#endif

            if (properties.count(L2DE_TILEDMAP_SMART_LAYER))
            {
#if L2DE_DEBUG
                ++tooMuchProperties;
#endif
                if (properties[L2DE_TILEDMAP_SMART_LAYER].GetType() == PropertyType::Int)
                {
                    layerId = properties[L2DE_TILEDMAP_SMART_LAYER].GetInt();
                    return true;
                }
                else
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the " L2DE_TILEDMAP_SMART_LAYER " property type is not valid. It should be Int.");
                }
            }

#if L2DE_DEBUG
            if (properties.size() >= tooMuchProperties)
            {
                L2DE_LOG_WARNING("TiledMapLoader: The Layer's and Object Layer's properties won't be processed. Except: " L2DE_TILEDMAP_SMART_LAYER);
            }
#endif
            return false;
        }

        void TiledMapLoader::CreateGameObjectFromLayerData(TiledMap& map, TiledMapTileset* tileset, int layerId, int imageId, int row, int column)
        {
            Transform transform(
                glm::vec2(
                    static_cast<float>(column) * map.GetTileWidth(),
                    static_cast<float>(row + 1) * map.GetTileHeight() - tileset->tiledSize.y),
                tileset->tiledSize
            );
            auto gameObject = GameObjectManager::GetInstance().CreateGameObject(transform);

            auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
                RendererMode::RENDER,
                *tileset->texture,
                layerId);
            renderer->data.uvMatrix = tileset->GetUV(imageId);

            std::map<std::string, System::Property> properties;
            if (tileset->commonProperties.size() > 0 ||
                tileset->uniqueProperties[tileset->GetLocalId(imageId)].size() > 0)
            {
                properties = tileset->commonProperties;
                for (auto& prop : tileset->uniqueProperties[tileset->GetLocalId(imageId)])
                {
                    properties[prop.first] = prop.second;
                }

                TiledMapLoader::AddColliderToGameObject(gameObject, properties);
            }

            if (properties.count(L2DE_TILEDMAP_SMART_GROUPNAME))
            {
                if (properties[L2DE_TILEDMAP_SMART_GROUPNAME].GetType() != PropertyType::String)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the " L2DE_TILEDMAP_SMART_GROUPNAME " should be string.");
                    map.gameObjects.push_back(gameObject);
                }
                else
                {
                    map.groupedGameObjects[
                        properties[L2DE_TILEDMAP_SMART_GROUPNAME].GetString()
                    ].push_back(gameObject);

                    properties.erase(L2DE_TILEDMAP_SMART_GROUPNAME);
                }
            }
            else
            {
                map.gameObjects.push_back(gameObject);
            }

            for (const auto& object : tileset->objects[tileset->GetLocalId(imageId)])
            {
                //The image is not supported in the tileds, even in Editor.
                switch (object.type)
                {
                case ObjectType::Point:
                {
                    const ObjectPoint* point = static_cast<const ObjectPoint*>(object.GetData());

                    GameObject* objectGameObject = GameObjectManager::GetInstance().CreateGameObject(
                        Transform(gameObject->transform.GetPosition() + point->position)
                    );
                    if (point->properties.size() > 0)
                        objectGameObject->AddComponent<PropertyComponent>(point->properties);
                }
                    break;
                case ObjectType::Box:
                    CreateColliderFromObjectItem<ObjectBox>(object, gameObject, properties);
                    break;
                case ObjectType::Ellipse:
                    CreateColliderFromObjectItem<ObjectEllipse>(object, gameObject, properties);
                    break;
                }
            }

            if (properties.size() > 0)
                gameObject->AddComponent<PropertyComponent>(std::move(properties));
        }

        void TiledMapLoader::CreateGameObjectFromObjectLayerData(
            TiledMap& map,
            const ObjectItem& objectItem,
            const std::vector<TiledMapTileset>& tilesets,
            int layerId
        )
        {
            GameObject* gameObject = nullptr;
            std::map<std::string, System::Property> properties = objectItem.GetData()->properties;

            switch (objectItem.type)
            {
            case ObjectType::Point:
            {
                const ObjectPoint* point = static_cast<const ObjectPoint*>(objectItem.GetData());

                gameObject = GameObjectManager::GetInstance().CreateGameObject(Transform(point->position));
            }
            break;
            case ObjectType::Box:
            {
                const ObjectBox* box = static_cast<const ObjectBox*>(objectItem.GetData());
                gameObject = GameObjectManager::GetInstance().CreateGameObject(Transform(box->position));

                TiledMapLoader::AddColliderToGameObject(gameObject, *box, properties, false);
            }
            break;
            case ObjectType::Ellipse:
            {
                const ObjectEllipse* ellipse = static_cast<const ObjectEllipse*>(objectItem.GetData());
                gameObject = GameObjectManager::GetInstance().CreateGameObject(Transform(ellipse->position));

                TiledMapLoader::AddColliderToGameObject(gameObject, *ellipse, properties, false);
            }
            break;
            case ObjectType::Image:
            {
                const ObjectImage* image = static_cast<const ObjectImage*>(objectItem.GetData());
                if (image->gid > 0)
                {
                    const TiledMapTileset* selectedTileset = nullptr;
                    for (auto& tileset : tilesets)
                    {
                        if (tileset.HasNumber(image->gid))
                        {
                            selectedTileset = &tileset;
                            break;
                        }
                    }

                    if (selectedTileset == nullptr)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the tile id " + std::to_string(image->gid) + " is not valid.");
                        return;
                    }

                    gameObject = GameObjectManager::GetInstance().CreateGameObject(Transform(image->position, image->size));
                    auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
                        RendererMode::RENDER,
                        *selectedTileset->texture,
                        layerId);
                    renderer->data.uvMatrix = selectedTileset->GetUV(image->gid);
                }
            }
            break;
            }

            if (gameObject != nullptr)
            {
                if (properties.count(L2DE_TILEDMAP_SMART_GROUPNAME))
                {
                    if (properties[L2DE_TILEDMAP_SMART_GROUPNAME].GetType() != PropertyType::String)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the " L2DE_TILEDMAP_SMART_GROUPNAME " should be string.");
                        map.gameObjects.push_back(gameObject);
                    }
                    else
                    {
                        map.groupedGameObjects[
                            properties[L2DE_TILEDMAP_SMART_GROUPNAME].GetString()
                        ].push_back(gameObject);

                        properties.erase(L2DE_TILEDMAP_SMART_GROUPNAME);
                    }
                }
                else
                {
                    map.gameObjects.push_back(gameObject);
                }

                if (properties.size() > 0)
                    gameObject->AddComponent<PropertyComponent>(properties);
            }
        }

        void TiledMapLoader::AddColliderToGameObject(GameObject* gameObject, std::map<std::string, Property>& properties)
        {
            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER))
            {
                if (properties[L2DE_TILEDMAP_SMART_COLLIDER].GetType() != PropertyType::String)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the " L2DE_TILEDMAP_SMART_COLLIDER " should be string.");
                    return;
                }

                auto type = ColliderType::DYNAMIC;
                auto mode = ColliderMode::COLLIDER;
                glm::vec2 offset(0.0f, 0.0f);
                int32_t maskLayer = ~0;

                if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC) &&
                    properties[L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC].GetBool())
                {
                    type = ColliderType::KINEMATIC;
                }

                if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER) &&
                    properties[L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER].GetBool())
                {
                    mode = ColliderMode::TRIGGER;
                }

                if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X))
                {
                    offset.x = properties[L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X].GetFloat();
                }

                if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y))
                {
                    offset.y = properties[L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y].GetFloat();
                }

                if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER))
                {
                    maskLayer = properties[L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER].GetInt();
                }

                bool addedCollider = false;
                if (properties[L2DE_TILEDMAP_SMART_COLLIDER].GetString() == L2DE_TILEDMAP_SMART_COLLIDER_VALUE_BOX)
                {
                    glm::vec2 size(gameObject->transform.GetScale());
                    if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_SIZE_X))
                    {
                        size.x = properties[L2DE_TILEDMAP_SMART_COLLIDER_SIZE_X].GetFloat();
                    }

                    if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_SIZE_Y))
                    {
                        size.y = properties[L2DE_TILEDMAP_SMART_COLLIDER_SIZE_Y].GetFloat();
                    }

                    gameObject->AddComponent<BoxColliderComponent>(size, type, mode, offset, maskLayer);
                    addedCollider = true;
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_SIZE_X);
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_SIZE_Y);
                }
                else if (properties[L2DE_TILEDMAP_SMART_COLLIDER].GetString() == L2DE_TILEDMAP_SMART_COLLIDER_VALUE_CIRCLE)
                {
                    float radius = gameObject->transform.GetScale().x / 2.0f;
                    if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_RADIUS))
                    {
                        radius = properties[L2DE_TILEDMAP_SMART_COLLIDER_RADIUS].GetFloat();
                    }

                    gameObject->AddComponent<CircleColliderComponent>(radius, type, mode, offset, maskLayer);
                    addedCollider = true;
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_RADIUS);
                }
                else
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the " L2DE_TILEDMAP_SMART_COLLIDER " has invalid value. Supported values: "
                        L2DE_TILEDMAP_SMART_COLLIDER_VALUE_BOX ", " L2DE_TILEDMAP_SMART_COLLIDER_VALUE_CIRCLE);
                }

                if (addedCollider)
                {
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER);
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC);
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER);
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X);
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y);
                    properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER);
                }
            }
        }

        void TiledMapLoader::AddColliderToGameObject(
            GameObject* gameObject,
            const ObjectBox& object,
            std::map<std::string, Property>& properties,
            bool useObjectPositionAsOffset
        )
        {
            auto size = object.size;
            auto type = ColliderType::DYNAMIC;
            auto mode = ColliderMode::COLLIDER;
            glm::vec2 offset(0.0f, 0.0f);
            if (useObjectPositionAsOffset)
                offset = object.position;
            int32_t maskLayer = ~0;

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC) &&
                properties[L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC].GetBool())
            {
                type = ColliderType::KINEMATIC;
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER) &&
                properties[L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER].GetBool())
            {
                mode = ColliderMode::TRIGGER;
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X))
            {
                offset.x = properties[L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X].GetFloat();
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y))
            {
                offset.y = properties[L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y].GetFloat();
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER))
            {
                maskLayer = properties[L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER].GetInt();
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_SIZE_X))
            {
                size.x = properties[L2DE_TILEDMAP_SMART_COLLIDER_SIZE_X].GetFloat();
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_SIZE_Y))
            {
                size.y = properties[L2DE_TILEDMAP_SMART_COLLIDER_SIZE_Y].GetFloat();
            }

            gameObject->AddComponent<BoxColliderComponent>(size, type, mode, offset, maskLayer);

            //The L2DE_TILEDMAP_SMART_COLLIDER is not used here, because the object is box.
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_SIZE_X);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_SIZE_Y);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER);
        }

        void TiledMapLoader::AddColliderToGameObject(
            GameObject* gameObject,
            const ObjectEllipse& object,
            std::map<std::string, Property>& properties,
            bool useObjectPositionAsOffset
        )
        {
            float radius = object.size.x;
            auto type = ColliderType::DYNAMIC;
            auto mode = ColliderMode::COLLIDER;
            glm::vec2 offset(0.0f, 0.0f);
            if (useObjectPositionAsOffset)
                offset = object.position;
            int32_t maskLayer = ~0;

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC) &&
                properties[L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC].GetBool())
            {
                type = ColliderType::KINEMATIC;
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER) &&
                properties[L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER].GetBool())
            {
                mode = ColliderMode::TRIGGER;
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X))
            {
                offset.x = properties[L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X].GetFloat();
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y))
            {
                offset.y = properties[L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y].GetFloat();
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER))
            {
                maskLayer = properties[L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER].GetInt();
            }

            if (properties.count(L2DE_TILEDMAP_SMART_COLLIDER_RADIUS))
            {
                radius = properties[L2DE_TILEDMAP_SMART_COLLIDER_RADIUS].GetFloat();
            }

            gameObject->AddComponent<CircleColliderComponent>(radius, type, mode, offset, maskLayer);

            //The L2DE_TILEDMAP_SMART_COLLIDER is not used here, because the object is box.
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_RADIUS);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_IS_KINEMATIC);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_IS_TRIGGER);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_X);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_OFFSET_Y);
            properties.erase(L2DE_TILEDMAP_SMART_COLLIDER_MASKLAYER);
        }
    }
}