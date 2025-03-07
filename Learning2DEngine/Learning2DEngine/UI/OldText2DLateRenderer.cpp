#include "OldText2DLateRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../Render/ShaderConstant.h"

#include "TextCharacterSet.h"

namespace Learning2DEngine
{
    using namespace Render;
    using namespace System;

    namespace UI
    {
        int OldText2DLateRenderer::referenceNumber = 0;
        Shader OldText2DLateRenderer::shader;

        GLuint OldText2DLateRenderer::vao = 0;
        GLuint OldText2DLateRenderer::vbo = 0;
        GLuint OldText2DLateRenderer::ebo = 0;

        OldText2DLateRenderer::OldText2DLateRenderer(
            GameObject* gameObject,
            const FontSizePair& fontSizePair,
            int layer,
            glm::vec4 color)
            :Component(gameObject), OldBaseRendererComponent(gameObject, layer), OldLateRendererComponent(gameObject, layer),
            fontSizePair(fontSizePair), text(""), color(color)
        {

        }

        OldText2DLateRenderer::OldText2DLateRenderer(
            GameObject* gameObject,
            const FontSizePair& fontSizePair,
            std::string text,
            int layer,
            glm::vec4 color)
            :Component(gameObject), OldBaseRendererComponent(gameObject, layer), OldLateRendererComponent(gameObject, layer),
            fontSizePair(fontSizePair), text(text), color(color)
        {

        }


        void OldText2DLateRenderer::Init()
        {
            OldLateRendererComponent::Init();

            // If nothing use it
            if (!OldText2DLateRenderer::referenceNumber)
            {
                InitShader();
                InitVao();
            }
            ++OldText2DLateRenderer::referenceNumber;
        }

        void OldText2DLateRenderer::Destroy()
        {
            OldLateRendererComponent::Destroy();

            --OldText2DLateRenderer::referenceNumber;
            // If nothing use it
            if (!OldText2DLateRenderer::referenceNumber)
            {
                glDeleteVertexArrays(1, &OldText2DLateRenderer::vao);
                glDeleteBuffers(1, &OldText2DLateRenderer::vbo);
                glDeleteBuffers(1, &OldText2DLateRenderer::ebo);
                glDeleteVertexArrays(1, &vao);
            }
        }

        void OldText2DLateRenderer::InitShader()
        {
            auto& resourceManager = ResourceManager::GetInstance();

            OldText2DLateRenderer::shader = resourceManager.IsShaderExist(ShaderConstant::TEXT2D_SHADER_NAME)
                ? resourceManager.GetShader(ShaderConstant::TEXT2D_SHADER_NAME)
                : resourceManager.LoadShader(
                    ShaderConstant::TEXT2D_SHADER_NAME,
                    ShaderConstant::TEXT2D_VERTEX_SHADER,
                    ShaderConstant::TEXT2D_FRAGMENT_SHADER);

            OldText2DLateRenderer::shader.Use();
            OldText2DLateRenderer::shader.SetInteger("characterTexture", 0);
        }

        void OldText2DLateRenderer::InitVao()
        {
            unsigned int indices[] = {
                0, 1, 3,
                1, 2, 3
            };

            glGenVertexArrays(1, &OldText2DLateRenderer::vao);
            glGenBuffers(1, &OldText2DLateRenderer::vbo);
            glGenBuffers(1, &OldText2DLateRenderer::ebo);
            glBindVertexArray(OldText2DLateRenderer::vao);
            glBindBuffer(GL_ARRAY_BUFFER, OldText2DLateRenderer::vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OldText2DLateRenderer::ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        glm::mat2 OldText2DLateRenderer::GetRotationMatrix()
        {
            float radians = glm::radians(gameObject->transform.rotation);

            return glm::mat2(
                glm::cos(radians), -glm::sin(radians),
                glm::sin(radians), glm::cos(radians)
            );
        }

        void OldText2DLateRenderer::Draw()
        {
            TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();
            textCharacterSet.Load(fontSizePair);

            CharacterMap& characterMap = textCharacterSet[fontSizePair];

            OldText2DLateRenderer::shader.Use();
            OldText2DLateRenderer::shader.SetVector4f("characterColor", color);
            OldText2DLateRenderer::shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao);

            glm::vec2 startPosition(gameObject->transform.position);
            glm::mat2 rotationMatrix = GetRotationMatrix();
            std::string::const_iterator c;
            for (c = text.begin(); c != text.end(); ++c)
            {
                const auto& ch = characterMap[*c];

                float chPositionX = ch.bearing.x * gameObject->transform.scale.x;
                float chPositionY = (characterMap['H'].bearing.y - ch.bearing.y) * gameObject->transform.scale.y;

                float chWidth = ch.size.x * gameObject->transform.scale.x;
                float chHeight = ch.size.y * gameObject->transform.scale.y;

                glm::vec2 a = rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY + chHeight);
                glm::vec2 b = rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY);
                glm::vec2 c = rotationMatrix * glm::vec2(chPositionX,           chPositionY);
                glm::vec2 d = rotationMatrix * glm::vec2(chPositionX,           chPositionY + chHeight);

                float vertices[4][4] = {
                    { startPosition.x + a.x, startPosition.y + a.y,   1.0f, 1.0f },
                    { startPosition.x + b.x, startPosition.y + b.y,   1.0f, 0.0f },
                    { startPosition.x + c.x, startPosition.y + c.y,   0.0f, 0.0f },
                    { startPosition.x + d.x, startPosition.y + d.y,   0.0f, 1.0f }
                };

                glBindTexture(GL_TEXTURE_2D, ch.textureId);

                glBindBuffer(GL_ARRAY_BUFFER, OldText2DLateRenderer::vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                startPosition += rotationMatrix *
                    glm::vec2(
                        // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
                        (ch.advance >> 6) * gameObject->transform.scale.x,
                        0);
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}