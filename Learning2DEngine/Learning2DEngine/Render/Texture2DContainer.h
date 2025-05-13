#pragma once

#include "Texture2D.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class Texture2DContainer
        {
        private:
            Texture2D* texture;
        public:
            Texture2DContainer() = default;
            virtual ~Texture2DContainer() = default;

            inline void SetTexture2D(Texture2D* texture)
            {
                this->texture = texture;
            }

            inline Texture2D* GetTexture()
            {
                return texture;
            }

            inline void ClearTexture()
            {
                texture = nullptr;
            }

            inline bool IsUseTexture() const
            {
                return texture != nullptr;
            }
        };
    }
}