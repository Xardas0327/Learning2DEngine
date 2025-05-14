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
            Texture2DContainer()
                : texture(nullptr)
            {

            }

            Texture2DContainer(const Texture2D& texture)
                : texture(new Texture2D(texture))
            {

            }

            virtual ~Texture2DContainer()
            {
                if (IsUseTexture())
                {
                    delete texture;
                }
            }

            void SetTexture(const Texture2D& texture)
            {
                if(this->texture != nullptr)
                    delete this->texture;

                this->texture = new Texture2D(texture);
            }

            inline Texture2D* GetTexture()
            {
                return texture;
            }

            inline const Texture2D* GetTexture() const
            {
                return texture;
            }

            inline void ClearTexture()
            {
                delete texture;
                texture = nullptr;
            }

            inline bool IsUseTexture() const
            {
                return texture != nullptr;
            }
        };
    }
}