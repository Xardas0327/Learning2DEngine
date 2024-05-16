#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace UI
    {
        // Please check freetype the documentation for bearing and advance.
        struct FreeTypeCharacter {
            unsigned int textureId;
            glm::ivec2   size;
            glm::ivec2   bearing;
            unsigned int advance;
        };
    }
}