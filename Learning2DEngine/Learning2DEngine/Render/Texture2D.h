#pragma once

#include <glad/glad.h>

#include "Texture2DSettings.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class Texture2D
        {
        private:
            unsigned int id;
        public:
            // Width of the loaded image in pixels
            unsigned int width;
            // Height of the loaded image in pixels
            unsigned int height;

            Texture2DSettings settings;
            Texture2D();
            Texture2D(const Texture2DSettings& settings);
            void Create(unsigned int width, unsigned int height, const unsigned char* data);
            void Destroy();
            void Bind() const;

            inline unsigned int GetId() const
            {
                return id;
            }
        };
    }
}