#include "OldSpriteRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../System/ResourceManager.h"
#include "../System/Game.h"

namespace Learning2DEngine
{
    namespace Render
    {
        int OldSpriteRenderer::referenceNumber = 0;
        Shader OldSpriteRenderer::shader;

        unsigned int OldSpriteRenderer::vao = 0;
        unsigned int OldSpriteRenderer::vbo = 0;
        unsigned int OldSpriteRenderer::ebo = 0;

        OldSpriteRenderer::OldSpriteRenderer(glm::vec3 c)
            : texture(nullptr), color(c), isInit(false)
        {

        }

        OldSpriteRenderer::OldSpriteRenderer(const OldSpriteRenderer& other)
            : texture(nullptr), color(other.color), isInit(false)
        {
            if (other.texture != nullptr)
            {
                texture = new Texture2D(*other.texture);
            }

            if (other.isInit)
                Init();
        }

        OldSpriteRenderer& OldSpriteRenderer::operator=(const OldSpriteRenderer& other)
        {
            if (this == &other)
                return *this;

            Destroy();

            if (IsUseTexture())
            {
                delete texture;
            }
            if (other.texture != nullptr)
            {
                texture = new Texture2D(*other.texture);
            }
            color = other.color;

            if (other.isInit)
                Init();

            return *this;
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
            if (isInit)
                return;

            isInit = true;

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
            if (!isInit)
                return;

            isInit = false;

            --OldSpriteRenderer::referenceNumber;
            // If nothing use it
            if (!OldSpriteRenderer::referenceNumber)
            {
                OldSpriteRenderer::shader.Destroy();

                glDeleteVertexArrays(1, &OldSpriteRenderer::vao);
                glDeleteBuffers(1, &OldSpriteRenderer::vbo);
                glDeleteBuffers(1, &OldSpriteRenderer::ebo);
            }
        }

        void OldSpriteRenderer::InitShader()
        {
            auto& resourceManager = System::ResourceManager::GetInstance();
            OldSpriteRenderer::shader = resourceManager.LoadShader("Learning2DEngine/Shaders/DefaultSprite.vs", "Learning2DEngine/Shaders/DefaultSprite.fs");
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

        void OldSpriteRenderer::Draw(const System::Transform& transform)
        {
            OldSpriteRenderer::shader.Use();
            glm::mat4 model = glm::mat4(1.0f);
            // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
            model = glm::translate(model, glm::vec3(transform.position, 0.0f));
            // move origin of rotation to center of quad
            model = glm::translate(model, glm::vec3(0.5f * transform.scale.x, 0.5f * transform.scale.y, 0.0f));
            // then rotate
            model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            // move origin back
            model = glm::translate(model, glm::vec3(-0.5f * transform.scale.x, -0.5f * transform.scale.y, 0.0f));
            // then rotate
            model = glm::scale(model, glm::vec3(transform.scale, 1.0f)); // last scale

            OldSpriteRenderer::shader.SetMatrix4("model", model);
            OldSpriteRenderer::shader.SetMatrix4("projection", System::Game::GetCameraProjection());
            OldSpriteRenderer::shader.SetVector3f("spriteColor", color);
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