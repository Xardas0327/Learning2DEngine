#include "Text2DLateRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../System/ResourceManager.h"
#include "../Render/ShaderConstant.h"

#include "TextCharacterSet.h"

namespace Learning2DEngine
{
    using namespace Render;
    using namespace System;

    namespace UI
    {
        int Text2DLateRenderer::referenceNumber = 0;
        Shader Text2DLateRenderer::shader;

        unsigned int Text2DLateRenderer::vao = 0;
        unsigned int Text2DLateRenderer::vbo = 0;
        unsigned int Text2DLateRenderer::ebo = 0;

        Text2DLateRenderer::Text2DLateRenderer(
            GameObject* gameObject,
            const Resolution& cameraResolution,
            const FontSizePair& fontSizePair,
            int layer,
            glm::vec3 color)
            :Component(gameObject), BaseRendererComponent(gameObject, layer), LateRendererComponent(gameObject, layer),
            isInit(false), cameraResolution(cameraResolution), fontSizePair(fontSizePair), text(""), color(color)
        {

        }

        Text2DLateRenderer::Text2DLateRenderer(
            GameObject* gameObject,
            const Resolution& cameraResolution,
            const FontSizePair& fontSizePair,
            std::string text,
            int layer,
            glm::vec3 color)
            :Component(gameObject), BaseRendererComponent(gameObject, layer), LateRendererComponent(gameObject, layer),
            isInit(false), cameraResolution(cameraResolution), fontSizePair(fontSizePair), text(text), color(color)
        {

        }


        void Text2DLateRenderer::Init()
        {
            if (isInit)
                return;

            isInit = true;

            LateRendererComponent::Init();

            // If nothing use it
            if (!Text2DLateRenderer::referenceNumber)
            {
                InitShader();
                InitVao();
            }
            ++Text2DLateRenderer::referenceNumber;
        }

        void Text2DLateRenderer::Destroy()
        {
            if (!isInit)
                return;

            isInit = false;

            LateRendererComponent::Destroy();

            --Text2DLateRenderer::referenceNumber;
            // If nothing use it
            if (!Text2DLateRenderer::referenceNumber)
            {
                glDeleteVertexArrays(1, &Text2DLateRenderer::vao);
                glDeleteBuffers(1, &Text2DLateRenderer::vbo);
                glDeleteBuffers(1, &Text2DLateRenderer::ebo);
                glDeleteVertexArrays(1, &vao);
            }
        }

        void Text2DLateRenderer::InitShader()
        {
            auto& resourceManager = ResourceManager::GetInstance();

            Text2DLateRenderer::shader = resourceManager.IsShaderExist(ShaderConstant::TEXT2D_SHADER_NAME)
                ? resourceManager.GetShader(ShaderConstant::TEXT2D_SHADER_NAME)
                : resourceManager.LoadShader(
                    ShaderConstant::TEXT2D_SHADER_NAME,
                    ShaderConstant::TEXT2D_VERTEX_SHADER,
                    ShaderConstant::TEXT2D_FRAGMENT_SHADER);

            Text2DLateRenderer::shader.Use();
            Text2DLateRenderer::shader.SetInteger("characterTexture", 0);
        }

        void Text2DLateRenderer::InitVao()
        {
            unsigned int indices[] = {
                0, 1, 3,
                1, 2, 3
            };

            glGenVertexArrays(1, &Text2DLateRenderer::vao);
            glGenBuffers(1, &Text2DLateRenderer::vbo);
            glGenBuffers(1, &Text2DLateRenderer::ebo);
            glBindVertexArray(Text2DLateRenderer::vao);
            glBindBuffer(GL_ARRAY_BUFFER, Text2DLateRenderer::vbo);
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

        void Text2DLateRenderer::Draw()
        {
            TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();
            textCharacterSet.Load(fontSizePair);

            CharacterMap& characterMap = textCharacterSet[fontSizePair];

            Text2DLateRenderer::shader.Use();
            Text2DLateRenderer::shader.SetVector3f("characterColor", color);
            Text2DLateRenderer::shader.SetMatrix4(
                "projection",
                glm::ortho(
                    0.0f,
                    static_cast<float>(cameraResolution.GetWidth()),
                    static_cast<float>(cameraResolution.GetHeight()),
                    0.0f));
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao);

            std::string::const_iterator c;
            int xPosition = gameObject->transform.position.x;
            for (c = text.begin(); c != text.end(); ++c)
            {
                auto& ch = characterMap[*c];

                float xpos = xPosition + ch.bearing.x * gameObject->transform.scale.x;
                float ypos = gameObject->transform.position.y + (characterMap['H'].bearing.y - ch.bearing.y) * gameObject->transform.scale.y;

                float w = ch.size.x * gameObject->transform.scale.x;
                float h = ch.size.y * gameObject->transform.scale.y;

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
                xPosition += (ch.advance >> 6) * gameObject->transform.scale.x;
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}