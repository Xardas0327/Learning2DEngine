#include "Text2DRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../System/ResourceManager.h"
#include "../DebugTool/Log.h"
#include "../Render/ShaderConstant.h"

#include "TextCharacterSet.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

namespace Learning2DEngine
{
    namespace UI
    {
        Text2DRenderer::Text2DRenderer() :
            vao(0), vbo(0), ebo(0), textShader()
        {

        }

        void Text2DRenderer::Init(const Render::Resolution& resolution)
        {
            auto& resourceManager = ResourceManager::GetInstance();

            textShader = resourceManager.IsShaderExist(ShaderConstant::TEXT2D_SHADER_NAME)
                ? resourceManager.GetShader(ShaderConstant::TEXT2D_SHADER_NAME)
                : resourceManager.LoadShader(
                    ShaderConstant::TEXT2D_SHADER_NAME,
                    ShaderConstant::TEXT2D_VERTEX_SHADER,
                    ShaderConstant::TEXT2D_FRAGMENT_SHADER);
            textShader.Use();
            textShader.SetMatrix4(
                "projection",
                glm::ortho(
                    0.0f,
                    static_cast<float>(resolution.GetWidth()),
                    static_cast<float>(resolution.GetHeight()),
                    0.0f));
            textShader.SetInteger("characterTexture", 0);

            unsigned int indices[] = {
                0, 1, 3,
                1, 2, 3
            };

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void Text2DRenderer::Terminate()
        {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ebo);
        }

        void Text2DRenderer::RenderText(const Text& text)
        {
			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();
            textCharacterSet.Load(text.fontSizePair);

            CharacterMap& characterMap = textCharacterSet[text.fontSizePair];

            textShader.Use();
            textShader.SetVector3f("characterColor", text.color);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao);

            std::string::const_iterator c;
            int xPosition = text.x;
            for (c = text.text.begin(); c != text.text.end(); ++c)
            {
                auto& ch = characterMap[*c];

                float xpos = xPosition + ch.bearing.x * text.scale;
                float ypos = text.y + (characterMap['H'].bearing.y - ch.bearing.y) * text.scale;

                float w = ch.size.x * text.scale;
                float h = ch.size.y * text.scale;

                float vertices[4][4] = {
                    { xpos + w, ypos + h,   1.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 0.0f },
                    { xpos,     ypos + h,   0.0f, 1.0f }
                };

                glBindTexture(GL_TEXTURE_2D, ch.textureId);

                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
                xPosition += (ch.advance >> 6) * text.scale;
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}