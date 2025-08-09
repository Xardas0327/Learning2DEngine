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

            inline void FlipUVx()
            {
                uvMatrix = glm::mat4x2{
                    uvMatrix[1].x, uvMatrix[1].y,
                    uvMatrix[0].x, uvMatrix[0].y,
                    uvMatrix[3].x, uvMatrix[3].y,
                    uvMatrix[2].x, uvMatrix[2].y,
                };
			}

            inline void FlipUVy()
            {
                uvMatrix = glm::mat4x2{
                    uvMatrix[3].x, uvMatrix[3].y,
                    uvMatrix[2].x, uvMatrix[2].y,
                    uvMatrix[1].x, uvMatrix[1].y,
                    uvMatrix[0].x, uvMatrix[0].y,
                };
            }
        };
    }
}