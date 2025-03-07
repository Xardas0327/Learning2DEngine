#include "OldSpriteRenderer.h"

#include "../System/ResourceManager.h"
#include "../System/Game.h"
#include "ShaderConstant.h"

namespace Learning2DEngine
{
    using namespace System;

    namespace Render
    {
        int OldSpriteRenderer::referenceNumber = 0;
        Shader OldSpriteRenderer::shader;

        GLuint OldSpriteRenderer::vao = 0;
        GLuint OldSpriteRenderer::vbo = 0;
        GLuint OldSpriteRenderer::ebo = 0;

        OldSpriteRenderer::OldSpriteRenderer(GameObject* gameObject, int layer, glm::vec4 color)
            : Component(gameObject), OldBaseRendererComponent(gameObject, layer), OldRendererComponent(gameObject, layer),
            texture(nullptr), color(color)
        {

        }

        OldSpriteRenderer::OldSpriteRenderer(GameObject* gameObject, const Texture2D& texture, int layer, glm::vec4 color)
            : Component(gameObject), OldBaseRendererComponent(gameObject, layer), OldRendererComponent(gameObject, layer),
            color(color), texture(new Texture2D(texture))
        {
        }

        OldSpriteRenderer::~OldSpriteRenderer()
        {
            if (IsUseTexture())
            {
                delete texture;
            }
        }

        void OldSpriteRenderer::Init()
        {
            OldRendererComponent::Init();

            // If nothing use it
            if (!OldSpriteRenderer::referenceNumber)
            {
                InitShader();
                InitVao();
            }
            ++OldSpriteRenderer::referenceNumber;
        }

        void OldSpriteRenderer::Destroy()
        {
            OldRendererComponent::Destroy();

            --OldSpriteRenderer::referenceNumber;
            // If nothing use it
            if (!OldSpriteRenderer::referenceNumber)
            {
                glDeleteVertexArrays(1, &OldSpriteRenderer::vao);
                glDeleteBuffers(1, &OldSpriteRenderer::vbo);
                glDeleteBuffers(1, &OldSpriteRenderer::ebo);
            }
        }

        void OldSpriteRenderer::InitShader()
        {
            auto& resourceManager = System::ResourceManager::GetInstance();
            OldSpriteRenderer::shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
                ? resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
                : resourceManager.LoadShader(
                    ShaderConstant::SPRITE_SHADER_NAME,
                    ShaderConstant::SPRITE_VERTEX_SHADER,
                    ShaderConstant::SPRITE_FRAGMENT_SHADER);

            OldSpriteRenderer::shader.Use();
            OldSpriteRenderer::shader.SetInteger("spriteTexture", 0);
        }

        void OldSpriteRenderer::InitVao()
        {
            float vertices[] = {
                // pos      // tex
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 1.0f,
            };

            unsigned int indices[] = {
               0, 1, 3,
               1, 2, 3
            };

            glGenVertexArrays(1, &OldSpriteRenderer::vao);
            glGenBuffers(1, &OldSpriteRenderer::vbo);
            glGenBuffers(1, &OldSpriteRenderer::ebo);
            glBindVertexArray(OldSpriteRenderer::vao);
            glBindBuffer(GL_ARRAY_BUFFER, OldSpriteRenderer::vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OldSpriteRenderer::ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void OldSpriteRenderer::Draw()
        {
            OldSpriteRenderer::shader.Use();

            OldSpriteRenderer::shader.SetMatrix4("model", gameObject->transform.GetModelMatrix());
            OldSpriteRenderer::shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
            OldSpriteRenderer::shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());

            OldSpriteRenderer::shader.SetVector4f("spriteColor", color);
            OldSpriteRenderer::shader.SetInteger("isUseTexture", IsUseTexture());

            if (IsUseTexture())
            {
                glActiveTexture(GL_TEXTURE0);
                texture->Bind();
            }

            glBindVertexArray(OldSpriteRenderer::vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}