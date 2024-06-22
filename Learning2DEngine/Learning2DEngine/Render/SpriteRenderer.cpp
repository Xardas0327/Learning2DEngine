#include "SpriteRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../System/ResourceManager.h"

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
            : uniqueShader(), texture(), color(c), projection(),
            isUseTexture(false), isUseDefaultShader(true), isInit(false)
        {

        }

        SpriteRenderer::SpriteRenderer(const Shader& s, const glm::mat4& p, glm::vec3 c)
            : uniqueShader(s), texture(), color(c), projection(p),
            isUseTexture(false), isUseDefaultShader(false), isInit(false)
        {

        }

        SpriteRenderer::SpriteRenderer(const SpriteRenderer& other)
            : uniqueShader(other.uniqueShader), texture(other.texture), color(other.color), 
            projection(other.projection), isUseTexture(other.isUseTexture), isUseDefaultShader(other.isUseDefaultShader), 
            isInit(false)
        {
            if (other.isInit)
                Init();
        }

        SpriteRenderer& SpriteRenderer::operator=(const SpriteRenderer& other)
        {
            Terminate();
            uniqueShader = other.uniqueShader;
            texture = other.texture;
            color = other.color;
            projection = other.projection;
            isUseTexture = other.isUseTexture;
            isUseDefaultShader = other.isUseDefaultShader;

            if (other.isInit)
                Init();

            return *this;
        }

        SpriteRenderer::~SpriteRenderer()
        {

        }

        void SpriteRenderer::Init()
        {
            if (isInit)
                return;

            isInit = true;

            if (isUseDefaultShader)
            {
                // If nothing use it
                if (!useDefaultShader)
                {
                    InitDefaultShader();
                }
                ++useDefaultShader;
            }

            // If nothing use it
            if (!useVao)
            {
                InitVao();
            }
            ++useVao;
        }

        void SpriteRenderer::Terminate()
        {
            if (!isInit)
                return;

            isInit = false;

            if (isUseDefaultShader)
            {
                --useDefaultShader;

                // If nothing use it
                if (!useDefaultShader)
                {
                    defaultShader.Destroy();
                }
            }

            --useVao;
            if (!useVao)
            {
                glDeleteVertexArrays(1, &vao);
                glDeleteBuffers(1, &vbo);
                glDeleteBuffers(1, &ebo);
            }
        }

        void SpriteRenderer::InitDefaultShader()
        {
            auto& resourceManager = System::ResourceManager::GetInstance();
            defaultShader = resourceManager.LoadShader("Learning2DEngine/Shaders/DefaultSprite.vs", "Learning2DEngine/Shaders/DefaultSprite.fs");
            defaultShader.SetInteger("spriteTexture", 0);
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

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
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
            Shader& selectedShader = useDefaultShader ? defaultShader : uniqueShader;

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
            selectedShader.SetMatrix4("projection", projection);
            selectedShader.SetVector3f("spriteColor", color);
            selectedShader.SetInteger("isUseTexture", isUseTexture);

            if (isUseTexture)
            {
                glActiveTexture(GL_TEXTURE0);
                texture.Bind();
            }

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
	}
}