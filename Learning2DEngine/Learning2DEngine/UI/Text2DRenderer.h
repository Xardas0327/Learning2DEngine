#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../System/Singleton.h"
#include "../Render/Shader.h"
#include "../Render/Resolution.h"
#include "Text.h"

namespace Learning2DEngine
{
    namespace UI
    {

        class Text2DRenderer : public virtual System::Singleton<Text2DRenderer>
        {
            friend class System::Singleton<Text2DRenderer>;
        private:
            unsigned int vao;
            unsigned int vbo;
            unsigned int ebo;
            Render::Shader textShader;

            Text2DRenderer();
        public:
            /// <summary>
            /// It should be called after RenderManager.
            /// </summary>
            void Init(const Render::Resolution& resolution);
            /// <summary>
            /// It should be terminated before RenderManager
            /// </summary>
            void Terminate();
            /// <summary>
            /// The blend should be enabled, where the source factor is GL_SRC_ALPHA
            /// and destination factor is GL_ONE_MINUS_SRC_ALPHA
            /// </summary>
            void RenderText(const Text& text);
        };
    }
}