#include "TiledMapLoader.h"

#include <sstream>
#include <exception>
#include <memory>
#include <GLFW/glfw3.h>
#include <rapidxml/rapidxml_utils.hpp>

#include "TiledMapConstant.h"
#include "../DebugTool/DebugMacro.h"
#include "../DebugTool/Log.h"
#include "../Render/RenderManager.h"
#include "../Render/SpriteRenderComponent.h"
#include "../System/ResourceManager.h"
#include "../System/GameObjectManager.h"
#include "../System/PropertyComponent.h"


namespace Learning2DEngine
{
    using namespace Render;
    using namespace System;

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
                auto objects = TiledMapLoader::LoadObjects(mapNode, folderPath, textureMap);
                TiledMapLoader::LoadLayers(map, mapNode, objects);

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

        std::vector<TiledMapObject> TiledMapLoader::LoadObjects(
            rapidxml::xml_node<>* mapNode,
            const std::string& folderPath,
            const std::map<std::string, std::string>& textureMap)
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

                if (TiledMapLoader::LoadObject(folderPath, sourceName, textureMap, object))
                    objects.push_back(object);
            }
            return objects;
        }

        bool TiledMapLoader::LoadObject(
            const std::string& folderPath,
            const std::string& sourceName,
            const std::map<std::string, std::string>& textureMap,
            TiledMapObject& tiledMapObject)
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
                    tiledMapObject.columns = static_cast<float>(std::atoi(attr->value()));
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

            return true;
        }

        void TiledMapLoader::LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, std::vector<TiledMapObject>& objects)
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
                                TiledMapObject* selectedObject = nullptr;
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
                                        static_cast<float>(row + 1) * map.GetTileHeight() - selectedObject->tiledSize.y),
                                    selectedObject->tiledSize
                                );
                                auto gameObject = gameObjectManager.CreateGameObject(transform);

                                int usedLayerId = useOverrideLayerId ? overrideLayerId : layerId;
                                auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
                                    RendererMode::RENDER,
                                    *selectedObject->texture,
                                    usedLayerId);
                                renderer->data.uvMatrix = selectedObject->GetUV(id);

                                if (selectedObject->commonProperties.size() > 0 ||
                                    selectedObject->uniqueProperties[selectedObject->GetLocalId(id)].size() > 0)
                                {
                                    auto properties = selectedObject->commonProperties;
                                    for (auto& prop : selectedObject->uniqueProperties[selectedObject->GetLocalId(id)])
                                    {
                                        properties[prop.first] = prop.second;
                                    }
                                    gameObject->AddComponent<PropertyComponent>(std::move(properties));
                                }

                                map.gameObjects.push_back(gameObject);
                            }

                        }
                        ++column;
                    }
                    ++row;
                }
                ++layerId;
            }
        }

        std::map<std::string, System::Property> TiledMapLoader::LoadProperties(rapidxml::xml_node<>* node, const std::string& folderPath)
        {
            std::map<std::string, Property> properties;
            if (node == nullptr)
                return properties;

            if (node->name() != L2DE_TILEDMAP_NODE_PROPERTIES)
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
                case Learning2DEngine::System::PropertyType::Bool:
                    properties.emplace(nameAttr->value(), Property(strcmp(valueAttr->value(), "true") == 0 ? true : false));
                    break;
                case Learning2DEngine::System::PropertyType::Color:
                    properties.emplace(nameAttr->value(), Property(TiledMapLoader::ConvertStringToColor(valueAttr->value())));
                    break;
                case Learning2DEngine::System::PropertyType::File:
                    properties.emplace(nameAttr->value(), Property(folderPath + valueAttr->value(), type));
                    break;
                case Learning2DEngine::System::PropertyType::Float:
                    properties.emplace(nameAttr->value(), Property(static_cast<float>(std::atof(valueAttr->value()))));
                    break;
                case Learning2DEngine::System::PropertyType::Int:
                    properties.emplace(nameAttr->value(), Property(std::atoi(valueAttr->value())));
                    break;
                case Learning2DEngine::System::PropertyType::String:
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

        bool TiledMapLoader::LoadMapBackground(rapidxml::xml_node<>* mapNode)
        {

            auto properties = LoadProperties(mapNode);
#if L2DE_DEBUG
            int tooMuchProperties = 1;
#endif

            if (properties.count(L2DE_TILEDMAP_SMART_LOADBACKGROUND))
            {
#if L2DE_DEBUG
                ++tooMuchProperties;
#endif
                if (properties[L2DE_TILEDMAP_SMART_LOADBACKGROUND].GetType() == PropertyType::Bool
                    && properties[L2DE_TILEDMAP_SMART_LOADBACKGROUND].GetBool())
                {
                    return true;
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
            auto properties = LoadProperties(layerNode);
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
                L2DE_LOG_WARNING("TiledMapLoader: The Layer's properties won't be processed. Except: " L2DE_TILEDMAP_SMART_LAYER);
            }
#endif
            return false;
        }
    }
}