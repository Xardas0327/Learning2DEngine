#include "CollisionChecker.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        glm::vec2 CollisionChecker::GetEdge(const BoxCollider& boxCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 boxHalfExtents(boxCollider.size.x / 2.0f, boxCollider.size.y / 2.0f);
            glm::vec2 clamped = glm::clamp(distanceBetweenCenters, -boxHalfExtents, boxHalfExtents);

            return boxCollider.GetCenter() + clamped;
        }

        glm::vec2 CollisionChecker::GetEdge(const CircleCollider& circleCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 edge = glm::normalize(distanceBetweenCenters) * circleCollider.radius;

            return circleCollider.GetCenter() + edge;
        }

        glm::vec2 CollisionChecker::GetEdge(const BaseBoxColliderComponent& boxCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 boxHalfExtents(boxCollider.size.x / 2.0f, boxCollider.size.y / 2.0f);
            glm::vec2 clamped = glm::clamp(distanceBetweenCenters, -boxHalfExtents, boxHalfExtents);

            return boxCollider.GetCenter() + clamped;
        }

        glm::vec2 CollisionChecker::GetEdge(const BaseCircleColliderComponent& circleCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 edge = glm::normalize(distanceBetweenCenters) * circleCollider.radius;

            return circleCollider.GetCenter() + edge;
        }

        CollisionData CollisionChecker::CheckCollision(const BoxCollider& collider1, const BoxCollider& collider2)
        {
            glm::vec2 box1Center = collider1.GetCenter();
            glm::vec2 halfSize1 = collider1.size / 2.0f;
            glm::vec2 box2Center = collider2.GetCenter();
            glm::vec2 halfSize2 = collider2.size / 2.0f;

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
                data.edge1 = CollisionChecker::GetEdge(collider1, box2Center - box1Center);
                data.edge2 = CollisionChecker::GetEdge(collider2, box1Center - box2Center);
            }

            return data;
        }

        CollisionData CollisionChecker::CheckCollision(const CircleCollider& collider1, const CircleCollider& collider2)
        {
            glm::vec2 distance = collider2.GetCenter() - collider1.GetCenter();
            CollisionData data;
            data.isCollided =
                // distance between 2 centers
                glm::length(distance)
                <=
                // their radius
                collider1.radius + collider2.radius;

            if (data.isCollided)
            {
                data.edge1 = GetEdge(collider1, distance);
                data.edge2 = GetEdge(collider2, -distance);
            }

            return data;
        }

        CollisionData CollisionChecker::CheckCollision(const CircleCollider& circleCollider, const BoxCollider& boxCollider)
        {
            glm::vec2 circleCenter = circleCollider.GetCenter();
            glm::vec2 boxCenter = boxCollider.GetCenter();

            glm::vec2 boxEdge = GetEdge(boxCollider, circleCenter - boxCenter);

            glm::vec2 distance = boxEdge - circleCenter;

            CollisionData data;
            data.isCollided = glm::length(distance) <= circleCollider.radius;

            if (data.isCollided)
            {
                data.edge1 = GetEdge(circleCollider, boxCenter - circleCenter);
                data.edge2 = boxEdge;
            }

            return data;
        }

        CollisionData CollisionChecker::CheckCollision(const BaseBoxColliderComponent& collider1, const BaseBoxColliderComponent& collider2)
        {
            glm::vec2 box1Center = collider1.GetCenter();
            glm::vec2 halfSize1 = collider1.size / 2.0f;
            glm::vec2 box2Center = collider2.GetCenter();
            glm::vec2 halfSize2 = collider2.size / 2.0f;

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
                data.edge1 = CollisionChecker::GetEdge(collider1, box2Center - box1Center);
                data.edge2 = CollisionChecker::GetEdge(collider2, box1Center - box2Center);
            }

            return data;
        }

        CollisionData CollisionChecker::CheckCollision(const BaseCircleColliderComponent& collider1, const BaseCircleColliderComponent& collider2)
        {
            glm::vec2 distance = collider2.GetCenter() - collider1.GetCenter();
            CollisionData data;
            data.isCollided =
                // distance between 2 centers
                glm::length(distance)
                <=
                // their radius
                collider1.radius + collider2.radius;

            if (data.isCollided)
            {
                data.edge1 = GetEdge(collider1, distance);
                data.edge2 = GetEdge(collider2, -distance);
            }

            return data;
        }

        CollisionData CollisionChecker::CheckCollision(const BaseBoxColliderComponent& boxCollider, const BaseCircleColliderComponent& circleCollider)
        {
            glm::vec2 circleCenter = circleCollider.GetCenter();
            glm::vec2 boxCenter = boxCollider.GetCenter();

            glm::vec2 boxEdge = GetEdge(boxCollider, circleCenter - boxCenter);

            glm::vec2 distance = boxEdge - circleCenter;

            CollisionData data;
            data.isCollided = glm::length(distance) <= circleCollider.radius;

            if (data.isCollided)
            {
                data.edge1 = GetEdge(circleCollider, boxCenter - circleCenter);
                data.edge2 = boxEdge;
            }

            return data;
        }

        CollisionData CollisionChecker::CheckCollision(const BaseCircleColliderComponent& circleCollider, const BaseBoxColliderComponent& boxCollider)
        {
            glm::vec2 circleCenter = circleCollider.GetCenter();
            glm::vec2 boxCenter = boxCollider.GetCenter();

            glm::vec2 boxEdge = GetEdge(boxCollider, circleCenter - boxCenter);

            glm::vec2 distance = boxEdge - circleCenter;

            CollisionData data;
            data.isCollided = glm::length(distance) <= circleCollider.radius;

            if (data.isCollided)
            {
                data.edge1 = GetEdge(circleCollider, boxCenter - circleCenter);
                data.edge2 = boxEdge;
            }

            return data;
        }
    }
}