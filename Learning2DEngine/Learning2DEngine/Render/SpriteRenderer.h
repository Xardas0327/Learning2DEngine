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
            bool isInit;

            /// <summary>
            /// It is counted, that how many SpriteRenderer use shader and voa.
            /// It is important, that the shader will be created, if it is used and
            /// it will be destroyed, if nothing use it.
            /// </summary>
            static int referenceNumber;
            static Shader shader;
            static unsigned int vao;
            static unsigned int vbo;
            static unsigned int ebo;

            void InitShader();
            void InitVao();

            inline bool IsUseTexture()
            {
                return texture != nullptr;
            }
        public:
            Texture2D* texture;
            glm::vec3 color;

            SpriteRenderer(glm::vec3 color = glm::vec3(1.0f));
            SpriteRenderer(const SpriteRenderer& other);
            SpriteRenderer& operator=(const SpriteRenderer& other);
            ~SpriteRenderer();

            void Init();
            void Destroy();

            void Draw(const System::Transform& transform);
        };
    }
}