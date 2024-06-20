#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace System
    {
        // Please check the documentation of FreeType for bearing and advance.
        struct FreeTypeCharacter {
            unsigned int textureId;
            glm::ivec2   size;
            glm::ivec2   bearing;
            unsigned int advance;
        };
    }
}