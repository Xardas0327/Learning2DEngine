#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace Physics
    {
        struct CollisionData
        {
            bool isCollisoned;
            glm::vec2 closestPointOfObject1;
            glm::vec2 closestPointOfObject2;

            CollisionData(bool isCollisoned = false,
                glm::vec2 closestPointOfObject1 = glm::vec2(0.0f, 0.0f),
                glm::vec2 closestPointOfObject2 = glm::vec2(0.0f, 0.0f))
                : isCollisoned(isCollisoned), closestPointOfObject1(closestPointOfObject1),
                closestPointOfObject2(closestPointOfObject2)
            {

            }
        };
    }
}