#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "OldRendererComponent.h"
#include "../System/GameObject.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class OldSpriteRenderer : public virtual OldRendererComponent
        {
            friend class System::GameObject;
        private:
            /// <summary>
            /// It is counted, that how many SpriteRenderer use shader and voa.
            /// It is important, that the shader will be created, if it is used and
            /// it will be destroyed, if nothing use it.
            /// </summary>
            static int referenceNumber;
            static Shader shader;
            static GLuint vao;
            static GLuint vbo;
            static GLuint ebo;

            void InitShader();
            void InitVao();
        protected:
            OldSpriteRenderer(System::GameObject* gameObject, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
            OldSpriteRenderer(System::GameObject* gameObject, const Texture2D& texture, int layer = 0, glm::vec4 color = glm::vec4(1.0f));

            void Init() override;
            void Destroy() override;
            void Draw() override;
        public:
            Texture2D* texture;
            glm::vec4 color;

            ~OldSpriteRenderer();

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