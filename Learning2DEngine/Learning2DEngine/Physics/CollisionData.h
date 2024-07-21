#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace Physics
    {
        struct CollisionData
        {
            bool isCollisoned;
            // The closest point of object 1 to object 2
            glm::vec2 edge1;
            // The closest point of object 2 to object 1
            glm::vec2 edge2;

            CollisionData(bool isCollisoned = false,
                glm::vec2 edge1 = glm::vec2(0.0f, 0.0f),
                glm::vec2 edge2 = glm::vec2(0.0f, 0.0f))
                : isCollisoned(isCollisoned), edge1(edge1),
                edge2(edge2)
            {

            }
        };
    }
}