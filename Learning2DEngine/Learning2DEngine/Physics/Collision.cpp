#include "Collision.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        glm::vec2 Collision::GetEdge(const BoxCollider& boxCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 boxHalfExtents(boxCollider.size.x / 2.0f, boxCollider.size.y / 2.0f);
            glm::vec2 clamped = glm::clamp(distanceBetweenCenters, -boxHalfExtents, boxHalfExtents);

            return boxCollider.GetCenter() + clamped;
        }

        glm::vec2 Collision::GetEdge(const CircleCollider& circleCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 edge = glm::normalize(distanceBetweenCenters) * circleCollider.radius;

            return circleCollider.GetCenter() + edge;
        }

        CollisionData Collision::IsCollisoned(const BoxCollider& collider1, const BoxCollider& collider2)
        {
            glm::vec2 box1Center = collider1.GetCenter();
            glm::vec2 halfSize1 = collider1.size / 2.0f;
            glm::vec2 box2Center = collider2.GetCenter();
            glm::vec2 halfSize2 = collider2.size / 2.0f;

            CollisionData data;
            data.isCollisoned =
                // x-axis
                box1Center.x + halfSize1.x >= box2Center.x - halfSize2.x
                && box2Center.x + halfSize2.x >= box1Center.x - halfSize1.x
                // Y-axis
                && box1Center.y + halfSize1.y >= box2Center.y - halfSize2.y
                && box2Center.y + halfSize2.y >= box1Center.y - halfSize1.y;

            if (data.isCollisoned)
            {
                data.edge1 = Collision::GetEdge(collider1, box2Center - box1Center);
                data.edge2 = Collision::GetEdge(collider2, box1Center - box2Center);
            }

            return data;
        }

        CollisionData Collision::IsCollisoned(const CircleCollider& collider1, const CircleCollider& collider2)
        {
            glm::vec2 distance = collider2.GetCenter() - collider1.GetCenter();
            CollisionData data;
            data.isCollisoned =
                // distance between 2 centers
                glm::length(distance)
                <=
                // their radius
                collider1.radius + collider2.radius;

            if (data.isCollisoned)
            {
                data.edge1 = GetEdge(collider1, distance);
                data.edge2 = GetEdge(collider2, -distance);
            }

            return data;
        }

        CollisionData Collision::IsCollisoned(const CircleCollider& circleCollider, const BoxCollider& boxCollider)
        {
            glm::vec2 circleCenter = circleCollider.GetCenter();
            glm::vec2 boxCenter = boxCollider.GetCenter();

            glm::vec2 boxEdge = GetEdge(boxCollider, circleCenter - boxCenter);

            glm::vec2 distance = boxEdge - circleCenter;

            CollisionData data;
            data.isCollisoned = glm::length(distance) <= circleCollider.radius;

            if (data.isCollisoned)
            {
                data.edge1 = GetEdge(circleCollider, boxCenter - circleCenter);
                data.edge2 = boxEdge;
            }

            return data;
        }
    }
}