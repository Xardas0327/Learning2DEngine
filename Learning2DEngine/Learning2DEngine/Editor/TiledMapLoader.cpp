#include "TiledMapLoader.h"

#include <exception>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include "TiledMapConstant.h"
#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
    namespace Editor
    {
        TiledMap TiledMapLoader::LoadFromFile(const char* filePath)
        {
            auto doc = new rapidxml::xml_document<>;
            try
            {
                rapidxml::file<> xmlFile(filePath);
                doc->parse<0>(xmlFile.data());
                TiledMap map;
                auto mapNode = doc->first_node(L2DE_TILEDMAP_NODE_MAP);
                if (mapNode == nullptr)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the map node is missing.");
                    return map;
                }

                map.version = mapNode->first_attribute(L2DE_TILEDMAP_ATTR_VERSION)->value();
                if (map.version != L2DE_TILEDMAP_SUPPORTED_VERSION)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the map version is not supported: "
                        + map.version + "\n Supported version: " + L2DE_TILEDMAP_SUPPORTED_VERSION);
                }

                map.tiledVersion = mapNode->first_attribute(L2DE_TILEDMAP_ATTR_TILEDVERSION)->value();

                delete doc;
                return map;
            }
            catch (std::exception& e)
            {
                delete doc;
                L2DE_LOG_ERROR(e.what());
                throw e;
            }
        }
    }
}