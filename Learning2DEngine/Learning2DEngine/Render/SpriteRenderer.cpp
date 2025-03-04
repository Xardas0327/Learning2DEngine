#include "SpriteRenderer.h"

#include "../System/ResourceManager.h"
#include "../System/Game.h"
#include "ShaderConstant.h"

namespace Learning2DEngine
{
    using namespace System;

    namespace Render
    {
        int SpriteRenderer::referenceNumber = 0;
        Shader SpriteRenderer::shader;

        GLuint SpriteRenderer::vao = 0;
        GLuint SpriteRenderer::vbo = 0;
        GLuint SpriteRenderer::ebo = 0;

        SpriteRenderer::SpriteRenderer(GameObject* gameObject, int layer, glm::vec4 color)
            : Component(gameObject), BaseRendererComponent(gameObject, layer), RendererComponent(gameObject, layer),
            texture(nullptr), color(color)
        {

        }

        SpriteRenderer::SpriteRenderer(GameObject* gameObject, const Texture2D& texture, int layer, glm::vec4 color)
            : Component(gameObject), BaseRendererComponent(gameObject, layer), RendererComponent(gameObject, layer),
            color(color), texture(new Texture2D(texture))
        {
        }

        SpriteRenderer::~SpriteRenderer()
        {
            if (IsUseTexture())
            {
                delete texture;
            }
        }

        void SpriteRenderer::Init()
        {
			RendererComponent::Init();

            // If nothing use it
            if (!SpriteRenderer::referenceNumber)
            {
                InitShader();
                InitVao();
            }
            ++SpriteRenderer::referenceNumber;
        }

        void SpriteRenderer::Destroy()
        {
            RendererComponent::Destroy();

            --SpriteRenderer::referenceNumber;
            // If nothing use it
            if (!SpriteRenderer::referenceNumber)
            {
                glDeleteVertexArrays(1, &SpriteRenderer::vao);
                glDeleteBuffers(1, &SpriteRenderer::vbo);
                glDeleteBuffers(1, &SpriteRenderer::ebo);
            }
        }

        void SpriteRenderer::InitShader()
        {
            auto& resourceManager = System::ResourceManager::GetInstance();
            SpriteRenderer::shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
                ? resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
                : resourceManager.LoadShader(
                    ShaderConstant::SPRITE_SHADER_NAME,
                    ShaderConstant::SPRITE_VERTEX_SHADER,
                    ShaderConstant::SPRITE_FRAGMENT_SHADER);

            SpriteRenderer::shader.Use();
            SpriteRenderer::shader.SetInteger("spriteTexture", 0);
        }

        void SpriteRenderer::InitVao()
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

            glGenVertexArrays(1, &SpriteRenderer::vao);
            glGenBuffers(1, &SpriteRenderer::vbo);
            glGenBuffers(1, &SpriteRenderer::ebo);
            glBindVertexArray(SpriteRenderer::vao);
            glBindBuffer(GL_ARRAY_BUFFER, SpriteRenderer::vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteRenderer::ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void SpriteRenderer::Draw()
        {
            SpriteRenderer::shader.Use();

            SpriteRenderer::shader.SetMatrix4("model", gameObject->transform.GetModelMatrix());
            SpriteRenderer::shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
            SpriteRenderer::shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());

            SpriteRenderer::shader.SetVector4f("spriteColor", color);
            SpriteRenderer::shader.SetInteger("isUseTexture", IsUseTexture());

            if (IsUseTexture())
            {
                glActiveTexture(GL_TEXTURE0);
                texture->Bind();
            }

            glBindVertexArray(SpriteRenderer::vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}