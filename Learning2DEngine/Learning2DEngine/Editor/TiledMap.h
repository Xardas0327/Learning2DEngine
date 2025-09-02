#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace Editor
    {
        class TiledMapLoader;

        class TiledMap final
        {
            friend class TiledMapLoader;
        private:
            std::string version;
            std::string tiledVersion;
            std::string orientation;
            int width;
            int height;
            int tileWidth;
            int tileHeight;
            bool infinite;
            glm::vec4 backgroundColor;

            TiledMap()
                : version(), tiledVersion(), orientation(), width(0), height(0),
                tileWidth(0), tileHeight(0), infinite(false), backgroundColor(0.0f)
            {

            }
        public:
            ~TiledMap() = default;

            inline const std::string& GetVersion() const
            {
                return version;
            }

            inline const std::string& GetTiledVersion() const
            {
                return tiledVersion;
            }

            inline const std::string& GetOrientation() const
            {
                return orientation;
            }

            inline int GetWidth() const
            {
                return width;
            }

            inline int GetHeight() const
            {
                return height;
            }

            inline int GetTileWidth() const
            {
                return tileWidth;
            }

            inline int GetTileHeight() const
            {
                return tileHeight;
            }

            inline bool IsInfinite() const
            {
                return infinite;
            }

            inline const glm::vec4& GetBackgroundColor() const
            {
                return backgroundColor;
            }
        };
    }
}