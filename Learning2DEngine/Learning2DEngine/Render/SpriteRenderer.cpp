#include "SpriteRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../System/ResourceManager.h"
#include "../System/Game.h"

namespace Learning2DEngine
{
	namespace Render
	{
        int SpriteRenderer::useDefaultShader = 0;
        Shader SpriteRenderer::defaultShader;

        int SpriteRenderer::useVao = 0;
        unsigned int SpriteRenderer::vao = 0;
        unsigned int SpriteRenderer::vbo = 0;
        unsigned int SpriteRenderer::ebo = 0;

        SpriteRenderer::SpriteRenderer(glm::vec3 c)
            : uniqueShader(nullptr), texture(nullptr), color(c), isInit(false)
        {

        }

        SpriteRenderer::SpriteRenderer(const Shader& s, glm::vec3 c)
            : texture(nullptr), color(c), isInit(false)
        {
            uniqueShader = new Shader(s);
        }

        SpriteRenderer::SpriteRenderer(const SpriteRenderer& other)
            : uniqueShader(nullptr), texture(nullptr), color(other.color), isInit(false)
        {
            if (other.uniqueShader != nullptr)
            {
                uniqueShader = new Shader(*other.uniqueShader);
            }
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
            if (IsUseUniqueShader())
            {
                delete uniqueShader;
            }
            if (other.uniqueShader != nullptr)
            {
                uniqueShader = new Shader(*other.uniqueShader);
            }

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
            if (IsUseUniqueShader())
            {
                delete uniqueShader;
            }

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

            if (!IsUseUniqueShader())
            {
                // If nothing use it
                if (!SpriteRenderer::useDefaultShader)
                {
                    InitDefaultShader();
                }
                ++SpriteRenderer::useDefaultShader;
            }

            // If nothing use it
            if (!SpriteRenderer::useVao)
            {
                InitVao();
            }
            ++SpriteRenderer::useVao;
        }

        void SpriteRenderer::Destroy()
        {
            if (!isInit)
                return;

            isInit = false;

            // It doesn't destroy the uniqueShader,
            // because other things could use it
            if (!IsUseUniqueShader())
            {
                --SpriteRenderer::useDefaultShader;

                // If nothing use it
                if (!SpriteRenderer::useDefaultShader)
                {
                    SpriteRenderer::defaultShader.Destroy();
                }
            }

            --SpriteRenderer::useVao;
            if (!SpriteRenderer::useVao)
            {
                glDeleteVertexArrays(1, &SpriteRenderer::vao);
                glDeleteBuffers(1, &SpriteRenderer::vbo);
                glDeleteBuffers(1, &SpriteRenderer::ebo);
            }
        }

        void SpriteRenderer::InitDefaultShader()
        {
            auto& resourceManager = System::ResourceManager::GetInstance();
            SpriteRenderer::defaultShader = resourceManager.LoadShader("Learning2DEngine/Shaders/DefaultSprite.vs", "Learning2DEngine/Shaders/DefaultSprite.fs");
            SpriteRenderer::defaultShader.SetInteger("spriteTexture", 0);
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
            Shader& selectedShader = IsUseUniqueShader() ?  *uniqueShader :SpriteRenderer::defaultShader;

            selectedShader.Use();
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

            selectedShader.SetMatrix4("model", model);
            selectedShader.SetMatrix4("projection", System::Game::GetCameraProjection());
            selectedShader.SetVector3f("spriteColor", color);
            selectedShader.SetInteger("isUseTexture", IsUseTexture());

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