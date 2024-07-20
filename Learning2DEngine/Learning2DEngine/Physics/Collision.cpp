#include "Collision.h"

#include <glm/glm.hpp>

namespace Learning2DEngine
{
    namespace Physics
    {
        bool Collision::IsCollisoned(const BoxCollider& collider1, const BoxCollider& collider2)
        {
            return
                // x-axis
                collider1.center.x + collider1.size.x / 2 >= collider2.center.x
                && collider2.center.x + collider2.size.x / 2 >= collider1.center.x
                // Y-axis
                && collider1.center.y + collider1.size.y / 2 >= collider2.center.y
                && collider2.center.y + collider2.size.y / 2 >= collider1.center.y;
        }

        bool Collision::IsCollisoned(const CircleCollider& collider1, const CircleCollider& collider2)
        {
            return
                // distance between 2 centers
                glm::length(collider1.center - collider2.center)
                <=
                // their radius
                collider1.radius + collider2.radius;
        }

        bool Collision::IsCollisoned(const CircleCollider& circleCollider, const BoxCollider& boxCollider)
        {
            glm::vec2 boxHalfExtents(boxCollider.size.x / 2, boxCollider.size.y / 2);
            glm::vec2 distanceFromCenters = circleCollider.center - boxCollider.center;

            glm::vec2 clamped = glm::clamp(distanceFromCenters, -boxHalfExtents, boxHalfExtents);
            glm::vec2 closestPointOfBoxToCircle = boxCollider.center + clamped;

            glm::vec2 distance = closestPointOfBoxToCircle - circleCollider.center;

            return glm::length(distance) < circleCollider.radius;
        }
    }
}