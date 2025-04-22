#include "CollisionHelper.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        glm::vec2 CollisionHelper::GetEdge(const BaseBoxColliderComponent& boxCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 boxHalfExtents(boxCollider.colliderSize.x / 2.0f, boxCollider.colliderSize.y / 2.0f);
            glm::vec2 clamped = glm::clamp(distanceBetweenCenters, -boxHalfExtents, boxHalfExtents);

            return boxCollider.GetColliderCenter() + clamped;
        }

        glm::vec2 CollisionHelper::GetEdge(const BaseCircleColliderComponent& circleCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 edge = glm::normalize(distanceBetweenCenters) * circleCollider.colliderRadius;

            return circleCollider.GetColliderCenter() + edge;
        }

        inline float CollisionHelper::GetLength2(glm::vec2 distance)
        {
            return distance.x * distance.x + distance.y * distance.y;
        }

        CollisionData CollisionHelper::CheckCollision(const BaseBoxColliderComponent& collider1, const BaseBoxColliderComponent& collider2)
        {
            glm::vec2 box1Center = collider1.GetColliderCenter();
            glm::vec2 halfSize1 = collider1.colliderSize / 2.0f;
            glm::vec2 box2Center = collider2.GetColliderCenter();
            glm::vec2 halfSize2 = collider2.colliderSize / 2.0f;

            CollisionData data;
            data.isCollided =
                // x-axis
                box1Center.x + halfSize1.x >= box2Center.x - halfSize2.x
                && box2Center.x + halfSize2.x >= box1Center.x - halfSize1.x
                // Y-axis
                && box1Center.y + halfSize1.y >= box2Center.y - halfSize2.y
                && box2Center.y + halfSize2.y >= box1Center.y - halfSize1.y;

            if (data.isCollided)
            {
                data.edge1 = CollisionHelper::GetEdge(collider1, box2Center - box1Center);
                data.edge2 = CollisionHelper::GetEdge(collider2, box1Center - box2Center);
            }

            return data;
        }

        CollisionData CollisionHelper::CheckCollision(const BaseCircleColliderComponent& collider1, const BaseCircleColliderComponent& collider2)
        {
            glm::vec2 distance = collider2.GetColliderCenter() - collider1.GetColliderCenter();
            float radius = collider1.colliderRadius + collider2.colliderRadius;
            CollisionData data;
            data.isCollided = CollisionHelper::GetLength2(distance) <= (radius * radius);

            if (data.isCollided)
            {
                data.edge1 = GetEdge(collider1, distance);
                data.edge2 = GetEdge(collider2, -distance);
            }

            return data;
        }

        CollisionData CollisionHelper::CheckCollision(const BaseCircleColliderComponent& circleCollider, const BaseBoxColliderComponent& boxCollider)
        {
            glm::vec2 circleCenter = circleCollider.GetColliderCenter();
            glm::vec2 boxCenter = boxCollider.GetColliderCenter();

            glm::vec2 boxEdge = GetEdge(boxCollider, circleCenter - boxCenter);

            glm::vec2 distance = boxEdge - circleCenter;

            CollisionData data;
            data.isCollided = CollisionHelper::GetLength2(distance) <= (circleCollider.colliderRadius * circleCollider.colliderRadius);

            if (data.isCollided)
            {
                data.edge1 = GetEdge(circleCollider, boxCenter - circleCenter);
                data.edge2 = boxEdge;
            }

            return data;
        }

        CollisionData CollisionHelper::CheckCollision(const BaseBoxColliderComponent& boxCollider, const BaseCircleColliderComponent& circleCollider)
        {
            CollisionData data = CollisionHelper::CheckCollision(circleCollider, boxCollider);

            if (data.isCollided)
            {
				glm::vec2 temp = data.edge1;
				data.edge1 = data.edge2;
				data.edge2 = temp;
            }

            return data;
        }
    }
}