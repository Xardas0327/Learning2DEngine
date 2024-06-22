#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "../System/Transform.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class SpriteRenderer
        {
        private:
            Shader uniqueShader;
            Texture2D texture;
            glm::vec3 color;
            bool isUseTexture;
            bool isUseDefaultShader;
            bool isInit;

            /// <summary>
            /// It is counted, that how many SpriteRenderer use the default shader.
            /// It is important, that the default shader will be created, if it is used and
            /// it will be destroyed, if nothing use it.
            /// </summary>
            static int useDefaultShader;
            static Shader defaultShader;

            /// <summary>
            /// It is counted, that how many SpriteRenderer use vao (and the vbo, ebo).
            /// It is important, that the they will be created, if they is used and
            /// they will be destroyed, if nothing use them.
            /// </summary>
            static int useVao;
            static unsigned int vao;
            static unsigned int vbo;
            static unsigned int ebo;

            void InitDefaultShader();
            void InitVao();
        public:
            SpriteRenderer(glm::vec3 color = glm::vec3(1.0f));
            SpriteRenderer(const Shader& shader, glm::vec3 color = glm::vec3(1.0f));
            SpriteRenderer(const SpriteRenderer& other);
            SpriteRenderer& operator=(const SpriteRenderer& other);
            ~SpriteRenderer();

            void Init();
            void Terminate();

            void Draw(const System::Transform& transform);

            inline Texture2D GetTexture()
            {
                return texture;
            }

            inline void ClearTexture()
            {
                isUseTexture = false;
            }

            inline void SetTexture(const Texture2D& texture)
            {
                isUseTexture = true;
                this->texture = texture;
            }

            inline glm::vec3 GetColor()
            {
                return color;
            }

            inline void SetColor(const glm::vec3& color)
            {
                this->color = color;
            }
        };
    }
}