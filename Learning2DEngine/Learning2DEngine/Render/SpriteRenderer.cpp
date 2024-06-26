#include "SpriteRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../System/ResourceManager.h"
#include "../System/Game.h"

namespace Learning2DEngine
{
	namespace Render
	{
        int SpriteRenderer::referenceNumber = 0;
        Shader SpriteRenderer::shader;

        unsigned int SpriteRenderer::vao = 0;
        unsigned int SpriteRenderer::vbo = 0;
        unsigned int SpriteRenderer::ebo = 0;

        SpriteRenderer::SpriteRenderer(glm::vec3 c)
            : texture(nullptr), color(c), isInit(false)
        {

        }

        SpriteRenderer::SpriteRenderer(const SpriteRenderer& other)
            : texture(nullptr), color(other.color), isInit(false)
        {
            if (other.texture != nullptr)
            {
                texture = new Texture2D(*other.texture);
            }

            if (other.isInit)
                Init();
        }

        SpriteRenderer& SpriteRenderer::operator=(const SpriteRenderer& other)
        {
            if(this == &other)
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

        SpriteRenderer::~SpriteRenderer()
        {
            if (IsUseTexture())
            {
                delete texture;
            }
        }

        void SpriteRenderer::Init()
        {
            if (isInit)
                return;

            isInit = true;

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
            if (!isInit)
                return;

            isInit = false;

            --SpriteRenderer::referenceNumber;
            // If nothing use it
            if (!SpriteRenderer::referenceNumber)
            {
                SpriteRenderer::shader.Destroy();

                glDeleteVertexArrays(1, &SpriteRenderer::vao);
                glDeleteBuffers(1, &SpriteRenderer::vbo);
                glDeleteBuffers(1, &SpriteRenderer::ebo);
            }
        }

        void SpriteRenderer::InitShader()
        {
            auto& resourceManager = System::ResourceManager::GetInstance();
            SpriteRenderer::shader = resourceManager.LoadShader("Learning2DEngine/Shaders/DefaultSprite.vs", "Learning2DEngine/Shaders/DefaultSprite.fs");
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

        void SpriteRenderer::Draw(const System::Transform& transform)
        {
            SpriteRenderer::shader.Use();
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

            SpriteRenderer::shader.SetMatrix4("model", model);
            SpriteRenderer::shader.SetMatrix4("projection", System::Game::GetCameraProjection());
            SpriteRenderer::shader.SetVector3f("spriteColor", color);
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