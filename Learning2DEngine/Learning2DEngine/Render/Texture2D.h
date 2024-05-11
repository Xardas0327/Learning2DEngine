#pragma once

#include <glad/glad.h>

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
            // Format of the texture object
            unsigned int internalFormat;
            // Format of the loaded image
            unsigned int imageFormat;
            // Wrapping mode on S axis
            unsigned int wrapS;
            // Wrapping mode on T axis
            unsigned int wrapT;
            // Filtering mode if texture pixels < screen pixels
            unsigned int filterMin;
            // Filtering mode if texture pixels > screen pixels
            unsigned int filterMax; 
            Texture2D();
            void Create(unsigned int width, unsigned int height, const unsigned char* data);
            void Destroy();
            void Bind() const;
            inline unsigned int GetId() { return id; }
        };
    }
}