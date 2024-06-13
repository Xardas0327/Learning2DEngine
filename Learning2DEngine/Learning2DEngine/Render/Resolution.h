#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Learning2DEngine
{
    namespace Render
    {
        class Resolution
        {
        private:
            int width;
            int height;
        public:
            Resolution()
                : width(0), height(0)
            {

            }

            Resolution(int w, int h)
                : width(w), height(h)
            {

            }

            inline int GetWidth() const
            {
                return width;
            }

            void SetWidth(int w)
            {
                width = w;
            }

            inline int GetHeight() const
            {
                return height;
            }

            void SetHeight(int h)
            {
                height = h;
            }

            inline std::string ToString() const
            {
                return std::to_string(width) + "x" + std::to_string(height);
            }

            inline glm::vec2 ToVec2() const
            {
                return glm::vec2(width, height);
            }
        };
    }
}