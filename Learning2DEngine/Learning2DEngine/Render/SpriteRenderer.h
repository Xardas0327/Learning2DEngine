#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "../System/GameObject.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class SpriteRenderer : public virtual Renderer
        {
            friend class System::GameObject;
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
        protected:
            SpriteRenderer(System::GameObject* gameObject, glm::vec3 color = glm::vec3(1.0f));
            SpriteRenderer(System::GameObject* gameObject, const Texture2D& texture, glm::vec3 color = glm::vec3(1.0f));

            void Init() override;
            void Destroy() override;
        public:
            Texture2D* texture;
            glm::vec3 color;

            ~SpriteRenderer();

            void Draw() override;

            inline bool IsUseTexture()
            {
                return texture != nullptr;
            }

            inline void ClearTexture()
            {
                if (IsUseTexture())
                {
                    delete texture;
                    texture = nullptr;
                }
            }
        };
    }
}