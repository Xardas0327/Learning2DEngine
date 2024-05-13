#pragma once

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../Render/Shader.h"
#include "FreeTypeCharacter.h"

namespace Learning2DEngine
{
    namespace UI
    {
        class Text2DRenderer
        {
        private:
            unsigned int vao, vbo;
            std::map<char, FreeTypeCharacter> characters;
            Render::Shader textShader;
        public:
            Text2DRenderer(unsigned int width, unsigned int height);
            void Load(std::string font, unsigned int fontSize);
            void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
        };
    }
}