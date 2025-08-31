#pragma once

#include <string>

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

            TiledMap();
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
        };
    }
}