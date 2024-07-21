#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace Physics
    {
        struct CollisionData
        {
            bool isCollisoned;
            // The closest point of object1 to object2
            glm::vec2 edge1;
            // The closest point of object2 to object1
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