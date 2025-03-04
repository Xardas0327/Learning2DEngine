#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../Render/LateRendererComponent.h"
#include "../Render/Shader.h"
#include "../Render/Resolution.h"
#include "../System/GameObject.h"

#include "FontSizePair.h"

namespace Learning2DEngine
{
    namespace UI
    {
        class Text2DLateRenderer : public virtual Render::LateRendererComponent
        {
            friend class System::GameObject;
        private:
            /// <summary>
            /// It is counted, that how many Text2DLateRenderer use shader and voa.
            /// It is important, that the shader will be created, if it is used and
            /// it will be destroyed, if nothing use it.
            /// </summary>
            static int referenceNumber;
            static  Render::Shader shader;
            static GLuint vao;
            static GLuint vbo;
            static GLuint ebo;

            void InitShader();
            void InitVao();
        protected:
            Text2DLateRenderer(
                System::GameObject* gameObject,
                const FontSizePair& fontSizePair,
                int layer = 0,
                glm::vec4 color = glm::vec4(1.0f));
            Text2DLateRenderer(
                System::GameObject* gameObject,
                const FontSizePair& fontSizePair,
                std::string text,
                int layer = 0,
                glm::vec4 color = glm::vec4(1.0f));

            void Init() override;
            void Destroy() override;
            void Draw() override;

            glm::mat2 GetRotationMatrix();
        public:
            FontSizePair fontSizePair;
            std::string text;
            glm::vec4 color;
        };
    }
}
