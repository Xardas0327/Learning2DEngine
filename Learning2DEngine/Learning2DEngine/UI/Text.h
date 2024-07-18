#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace UI
    {
        // First is the font name.
        // Second is the size.
        typedef std::pair<std::string, unsigned int> FontSizePair;

        struct Text {
            FontSizePair fontSizePair;
            std::string text;
            float x = 0;
            float y = 0;
            float scale = 1.0f;
            glm::vec3 color = glm::vec3(1.0f);
        };
    }
}