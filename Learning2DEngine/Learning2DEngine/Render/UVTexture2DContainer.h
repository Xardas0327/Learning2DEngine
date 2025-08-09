#pragma once

#include <glm/glm.hpp>

#include "Texture2DContainer.h"

namespace Learning2DEngine
{
    namespace Render
    {
#define L2DE_Texture2D_UV_DEFAULT glm::mat4x2 { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f}

        class UVTexture2DContainer : public Texture2DContainer
        {
        public:
            /// <summary>
            /// The texture coordinate order:
            /// Top Left,
            /// Top Right,
            /// Bottom Right,
            /// Bottom Left
            /// </summary>
            glm::mat4x2 uvMatrix;

            UVTexture2DContainer(glm::mat4x2 uvMatrix = L2DE_Texture2D_UV_DEFAULT)
                : Texture2DContainer(), uvMatrix(uvMatrix)
            {

            }

            // It will not copy the texture, it will use texture reference.
            UVTexture2DContainer(const Texture2D& texture, glm::mat4x2 uvMatrix = L2DE_Texture2D_UV_DEFAULT)
                : Texture2DContainer(texture), uvMatrix(uvMatrix)
            {

            }
        };
    }
}