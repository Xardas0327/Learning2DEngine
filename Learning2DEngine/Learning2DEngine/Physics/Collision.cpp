#include "Collision.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        const glm::vec2 VECTOR_UP = glm::vec2(0.0f, 1.0f);
        const glm::vec2 VECTOR_DOWN = glm::vec2(0.0f, -1.0f);
        const glm::vec2 VECTOR_LEFT = glm::vec2(-1.0f, 0.0f);
        const glm::vec2 VECTOR_RIGHT = glm::vec2(1.0f, 0.0f);

        glm::vec2 Collision::GetEdge(const BoxCollider& boxCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 boxHalfExtents(boxCollider.size.x / 2, boxCollider.size.y / 2);
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
            CollisionData data;
            data.isCollisoned =
                // x-axis
                collider1.GetCenter().x + collider1.size.x / 2 >= collider1.GetCenter().x
                && collider1.GetCenter().x + collider2.size.x / 2 >= collider1.GetCenter().x
                // Y-axis
                && collider1.GetCenter().y + collider1.size.y / 2 >= collider1.GetCenter().y
                && collider1.GetCenter().y + collider2.size.y / 2 >= collider1.GetCenter().y;

            if (data.isCollisoned)
            {
                data.edge1 = Collision::GetEdge(collider1, collider2.GetCenter() - collider1.GetCenter());
                data.edge2 = Collision::GetEdge(collider2, collider1.GetCenter() - collider2.GetCenter());
            }

            return data;
        }

        CollisionData Collision::IsCollisoned(const CircleCollider& collider1, const CircleCollider& collider2)
        {
            glm::vec2 distance1 = collider2.GetCenter() - collider1.GetCenter();
            CollisionData data;
            data.isCollisoned =
                // distance between 2 centers
                glm::length(distance1)
                <=
                // their radius
                collider1.radius + collider2.radius;

            if (data.isCollisoned)
            {
                data.edge1 = GetEdge(collider1, distance1);
                data.edge2 = GetEdge(collider2, collider1.GetCenter() - collider2.GetCenter());
            }

            return data;
        }

        CollisionData Collision::IsCollisoned(const CircleCollider& circleCollider, const BoxCollider& boxCollider)
        {
            glm::vec2 boxEdge = GetEdge(boxCollider, circleCollider.GetCenter() - boxCollider.GetCenter());

            glm::vec2 distance = boxEdge - circleCollider.GetCenter();

            CollisionData data;
            data.isCollisoned = glm::length(distance) <= circleCollider.radius;

            if (data.isCollisoned)
            {
                data.edge1 = GetEdge(circleCollider, boxCollider.GetCenter() - circleCollider.GetCenter());
                data.edge2 = boxEdge;
            }

            return data;
        }
        void Collision::FixObjectAfterBoxCollision(const Collider* collider, glm::vec2 differenceVector)
        {
            float dotProductLeft = glm::dot(glm::normalize(differenceVector), Collision::VECTOR_LEFT);
            float dotProductRight = glm::dot(glm::normalize(differenceVector), Collision::VECTOR_RIGHT);
            Direction dX = dotProductLeft > dotProductRight ? LEFT : RIGHT;
            float dotProductX = dotProductLeft > dotProductRight ? dotProductLeft : dotProductRight;

            float dotProductUp = glm::dot(glm::normalize(differenceVector), Collision::VECTOR_UP);
            float dotProductDown = glm::dot(glm::normalize(differenceVector), Collision::VECTOR_DOWN);
            Direction dY = dotProductUp > dotProductDown ? UP : DOWN;
            float dotProductY = dotProductUp > dotProductDown ? dotProductUp : dotProductDown;

            if (dotProductX == dotProductY)
            {
                collider->rigidbody->velocity = -collider->rigidbody->velocity;
            }
            if (dotProductX > dotProductY)
            {
                collider->rigidbody->velocity.x = -collider->rigidbody->velocity.x;
            }
            else // (dotProductX < dotProductY)
            {
                collider->rigidbody->velocity.y = -collider->rigidbody->velocity.y;
            }

            if (dotProductX >= dotProductY)
            {
                float penetrationX = collider->GetCenter().x - std::abs(differenceVector.x);
                if (dX == LEFT)
                    collider->gameObject->transform.position.x += penetrationX;
                else
                    collider->gameObject->transform.position.x -= penetrationX;
            }

            if (dotProductX <= dotProductY)
            {
                float penetrationY = collider->GetCenter().y - std::abs(differenceVector.y);
                if (dY == UP)
                    collider->gameObject->transform.position.y += penetrationY;
                else
                    collider->gameObject->transform.position.y -= penetrationY;
            }
        }

        void Collision::FixObjectAfterCircleCollision(const Collider* collider, glm::vec2 hitPoint, glm::vec2 differenceVector)
        {
            collider->gameObject->transform.position += differenceVector;

            auto wallNormal = glm::normalize(hitPoint);
            auto velocityNormal = glm::normalize(collider->rigidbody->velocity);

            auto dotProduct = glm::dot(velocityNormal, wallNormal);
            auto incidenceAngle = glm::acos(dotProduct);
            auto reboundAngle = 3.14f - incidenceAngle;

            collider->rigidbody->velocity = wallNormal * glm::cos(reboundAngle) * glm::length(collider->rigidbody->velocity);
        }

        bool Collision::DoCollision(const BoxCollider& collider1, const BoxCollider& collider2)
        {
            auto data = IsCollisoned(collider1, collider2);

            if (!data.isCollisoned || (collider1.IsFrozen() && collider2.IsFrozen()))
                return data.isCollisoned;

            if (!collider1.IsFrozen())
            {
                auto differenceVector =  collider1.GetCenter() - data.edge2;
                if (!collider2.IsFrozen())
                    differenceVector /= 2;

                Collision::FixObjectAfterBoxCollision(&collider1, differenceVector);
            }

            if (!collider2.IsFrozen())
            {
                auto differenceVector = collider2.GetCenter() - data.edge1;
                if (!collider1.IsFrozen())
                    differenceVector /= 2;

                Collision::FixObjectAfterBoxCollision(&collider2, differenceVector);
            }

            return data.isCollisoned;
        }

        bool Collision::DoCollision(const CircleCollider& collider1, const CircleCollider& collider2)
        {
            auto data = IsCollisoned(collider1, collider2);

            if (!data.isCollisoned || (collider1.IsFrozen() && collider2.IsFrozen()))
                return data.isCollisoned;

            if (!collider1.IsFrozen())
            {
                auto differenceVector = data.edge1 - collider2.GetCenter();
                if (!collider2.IsFrozen())
                    differenceVector /= 2;

                FixObjectAfterCircleCollision(&collider1, data.edge2, differenceVector);
            }

            if (!collider2.IsFrozen())
            {
                auto differenceVector = data.edge2 - collider1.GetCenter();
                if (!collider1.IsFrozen())
                    differenceVector /= 2;

                FixObjectAfterCircleCollision(&collider2, data.edge1, differenceVector);
            }

            return data.isCollisoned;
        }

        bool Collision::DoCollision(const CircleCollider& circleCollider, const BoxCollider& boxCollider)
        {
            auto data = IsCollisoned(circleCollider, boxCollider);

            if (!data.isCollisoned || (circleCollider.IsFrozen() && boxCollider.IsFrozen()))
                return data.isCollisoned;

            if (!circleCollider.IsFrozen())
            {
                auto differenceVector = circleCollider.GetCenter() -data.edge2;
                if (!boxCollider.IsFrozen())
                    differenceVector /= 2;

                Collision::FixObjectAfterBoxCollision(&circleCollider, differenceVector);
            }

            if (!boxCollider.IsFrozen())
            {
                auto differenceVector = data.edge2 - circleCollider.GetCenter();
                if (!circleCollider.IsFrozen())
                    differenceVector /= 2;

                FixObjectAfterCircleCollision(&boxCollider, data.edge2, differenceVector);
            }

            return data.isCollisoned;
        }

        bool Collision::DoCollision(const BoxCollider& boxCollider, const CircleCollider& circleCollider)
        {
            return Collision::DoCollision(circleCollider, boxCollider);
        }
    }
}