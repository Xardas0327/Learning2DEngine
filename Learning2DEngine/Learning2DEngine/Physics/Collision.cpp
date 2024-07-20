#include "Collision.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        const glm::vec2 VECTOR_UP = glm::vec2(0.0f, 1.0f);
        const glm::vec2 VECTOR_DOWN = glm::vec2(0.0f, -1.0f);
        const glm::vec2 VECTOR_LEFT = glm::vec2(-1.0f, 0.0f);
        const glm::vec2 VECTOR_RIGHT = glm::vec2(1.0f, 0.0f);

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
                glm::vec2 distanceBetweenCenters = collider1.GetCenter() - collider2.GetCenter();

                glm::vec2 boxHalfExtents1(collider1.size.x / 2, collider1.size.y / 2);
                glm::vec2 boxHalfExtents2(collider2.size.x / 2, collider2.size.y / 2);

                glm::vec2 clamped1 = glm::clamp(distanceBetweenCenters, -boxHalfExtents1, boxHalfExtents1);
                glm::vec2 clamped2 = glm::clamp(distanceBetweenCenters, -boxHalfExtents2, boxHalfExtents2);

                data.closestPointOfObject1 = collider1.GetCenter() + clamped1;
                data.closestPointOfObject2 = collider2.GetCenter() + clamped2;
            }

            return data;
        }

        CollisionData Collision::IsCollisoned(const CircleCollider& collider1, const CircleCollider& collider2)
        {
            glm::vec2 distanceBetweenCenters = collider1.GetCenter() - collider2.GetCenter();
            CollisionData data;
            data.isCollisoned =
                // distance between 2 centers
                glm::length(distanceBetweenCenters)
                <=
                // their radius
                collider1.radius + collider2.radius;

            if (data.isCollisoned)
            {
                glm::vec2 distance1 = collider2.GetCenter() - collider1.GetCenter();
                glm::vec2 distance2 = collider1.GetCenter() - collider2.GetCenter();

                glm::vec2 edge1 = glm::normalize(distance1) * collider1.radius;
                glm::vec2 edge2 = glm::normalize(distance2) * collider1.radius;

                data.closestPointOfObject1 = collider1.GetCenter() + edge1;
                data.closestPointOfObject2 = collider2.GetCenter() + edge2;
            }

            return data;
        }

        CollisionData Collision::IsCollisoned(const CircleCollider& circleCollider, const BoxCollider& boxCollider)
        {
            glm::vec2 boxHalfExtents(boxCollider.size.x / 2, boxCollider.size.y / 2);
            glm::vec2 distanceFromBoxToCircle = circleCollider.GetCenter() - boxCollider.GetCenter();
            glm::vec2 distanceFromCircleToBox = boxCollider.GetCenter() - circleCollider.GetCenter();

            glm::vec2 clamped = glm::clamp(distanceFromBoxToCircle, -boxHalfExtents, boxHalfExtents);
            glm::vec2 closestPointOfBoxToCircle = boxCollider.GetCenter() + clamped;

            glm::vec2 edge = glm::normalize(distanceFromCircleToBox) * circleCollider.radius;

            glm::vec2 distance = closestPointOfBoxToCircle - circleCollider.GetCenter();
            glm::vec2 closestPointOfCircleToBox= circleCollider.GetCenter() + edge;

            bool isCollisoned = glm::length(distance) <= circleCollider.radius;

            return CollisionData(isCollisoned, closestPointOfBoxToCircle, closestPointOfCircleToBox);
        }

        bool Collision::DoCollision(const BoxCollider& collider1, const BoxCollider& collider2)
        {
            auto data = IsCollisoned(collider1, collider2);

            if (!data.isCollisoned || (collider1.IsFrozen() && collider2.IsFrozen()))
                return data.isCollisoned;

            if (!collider1.IsFrozen())
            {
                auto differenceVector = data.closestPointOfObject2 - collider1.GetCenter();
                if (!collider2.IsFrozen())
                    differenceVector /= 2;

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
                    collider1.rigidbody->velocity = -collider1.rigidbody->velocity;

                    float penetrationX = collider1.GetCenter().x - std::abs(differenceVector.x);
                    if (dX == LEFT)
                        collider1.gameObject->transform.position.x += penetrationX;
                    else
                        collider1.gameObject->transform.position.x -= penetrationX;

                    float penetrationY = collider1.GetCenter().y - std::abs(differenceVector.y);
                    if (dY == UP)
                        collider1.gameObject->transform.position.y += penetrationY;
                    else
                        collider1.gameObject->transform.position.y -= penetrationY;
                }
                if (dotProductX > dotProductY)
                {
                    collider1.rigidbody->velocity.x = -collider1.rigidbody->velocity.x;

                    float penetrationX = collider1.GetCenter().x - std::abs(differenceVector.x);
                    if (dX == LEFT)
                        collider1.gameObject->transform.position.x += penetrationX;
                    else
                        collider1.gameObject->transform.position.x -= penetrationX;
                }
                else // (dotProductX < dotProductY)
                {
                    collider1.rigidbody->velocity.y = -collider1.rigidbody->velocity.y;
                    float penetrationY = collider1.GetCenter().y - std::abs(differenceVector.y);
                    if (dY == UP)
                        collider1.gameObject->transform.position.y += penetrationY;
                    else
                        collider1.gameObject->transform.position.y -= penetrationY;
                }
            }

            if (!collider2.IsFrozen())
            {
                auto differenceVector = data.closestPointOfObject1 - collider2.GetCenter();
                if (!collider1.IsFrozen())
                    differenceVector /= 2;

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
                    collider2.rigidbody->velocity = -collider2.rigidbody->velocity;

                    float penetrationX = collider2.GetCenter().x - std::abs(differenceVector.x);
                    if (dX == LEFT)
                        collider2.gameObject->transform.position.x += penetrationX;
                    else
                        collider2.gameObject->transform.position.x -= penetrationX;

                    float penetrationY = collider2.GetCenter().y - std::abs(differenceVector.y);
                    if (dY == UP)
                        collider2.gameObject->transform.position.y += penetrationY;
                    else
                        collider2.gameObject->transform.position.y -= penetrationY;
                }
                if (dotProductX > dotProductY)
                {
                    collider2.rigidbody->velocity.x = -collider2.rigidbody->velocity.x;

                    float penetrationX = collider2.GetCenter().x - std::abs(differenceVector.x);
                    if (dX == LEFT)
                        collider2.gameObject->transform.position.x += penetrationX;
                    else
                        collider2.gameObject->transform.position.x -= penetrationX;
                }
                else // (dotProductX < dotProductY)
                {
                    collider2.rigidbody->velocity.y = -collider2.rigidbody->velocity.y;
                    float penetrationY = collider2.GetCenter().y - std::abs(differenceVector.y);
                    if (dY == UP)
                        collider2.gameObject->transform.position.y += penetrationY;
                    else
                        collider2.gameObject->transform.position.y -= penetrationY;
                }
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
                auto differenceVector = data.closestPointOfObject1 - collider2.GetCenter();
                if (!collider2.IsFrozen())
                    differenceVector /= 2;

                collider1.gameObject->transform.position += differenceVector;


                auto wallNormal = glm::normalize(data.closestPointOfObject2);
                auto velocityNormal = glm::normalize(collider1.rigidbody->velocity);

                auto dotProduct = glm::dot(velocityNormal, wallNormal);
                auto incidenceAngle = glm::acos(dotProduct);
                auto reboundAngle = 3.14f - incidenceAngle;

                collider1.rigidbody->velocity = wallNormal * glm::cos(reboundAngle) * glm::length(collider1.rigidbody->velocity);
            }

            if (!collider2.IsFrozen())
            {
                auto differenceVector = data.closestPointOfObject2 - collider1.GetCenter();
                if (!collider1.IsFrozen())
                    differenceVector /= 2;

                collider2.gameObject->transform.position += differenceVector;

                auto wallNormal = glm::normalize(data.closestPointOfObject1);
                auto velocityNormal = glm::normalize(collider2.rigidbody->velocity);

                auto dotProduct = glm::dot(velocityNormal, wallNormal);
                auto incidenceAngle = glm::acos(dotProduct);
                auto reboundAngle = 3.14f - incidenceAngle;

                collider2.rigidbody->velocity = wallNormal * glm::cos(reboundAngle) * glm::length(collider2.rigidbody->velocity);
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
                auto differenceVector = data.closestPointOfObject2 - circleCollider.GetCenter();
                if (!boxCollider.IsFrozen())
                    differenceVector /= 2;

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
                    circleCollider.rigidbody->velocity = -circleCollider.rigidbody->velocity;

                    float penetrationX = circleCollider.GetCenter().x - std::abs(differenceVector.x);
                    if (dX == LEFT)
                        circleCollider.gameObject->transform.position.x += penetrationX;
                    else
                        circleCollider.gameObject->transform.position.x -= penetrationX;

                    float penetrationY = circleCollider.GetCenter().y - std::abs(differenceVector.y);
                    if (dY == UP)
                        circleCollider.gameObject->transform.position.y += penetrationY;
                    else
                        circleCollider.gameObject->transform.position.y -= penetrationY;
                }
                if (dotProductX > dotProductY)
                {
                    circleCollider.rigidbody->velocity.x = -circleCollider.rigidbody->velocity.x;

                    float penetrationX = circleCollider.GetCenter().x - std::abs(differenceVector.x);
                    if (dX == LEFT)
                        circleCollider.gameObject->transform.position.x += penetrationX;
                    else
                        circleCollider.gameObject->transform.position.x -= penetrationX;
                }
                else // (dotProductX < dotProductY)
                {
                    circleCollider.rigidbody->velocity.y = -circleCollider.rigidbody->velocity.y;
                    float penetrationY = circleCollider.GetCenter().y - std::abs(differenceVector.y);
                    if (dY == UP)
                        circleCollider.gameObject->transform.position.y += penetrationY;
                    else
                        circleCollider.gameObject->transform.position.y -= penetrationY;
                }
            }

            if (!boxCollider.IsFrozen())
            {
                auto differenceVector = data.closestPointOfObject2 - circleCollider.GetCenter();
                if (!circleCollider.IsFrozen())
                    differenceVector /= 2;

                circleCollider.gameObject->transform.position += differenceVector;

                auto wallNormal = glm::normalize(data.closestPointOfObject1);
                auto velocityNormal = glm::normalize(boxCollider.rigidbody->velocity);

                auto dotProduct = glm::dot(velocityNormal, wallNormal);
                auto incidenceAngle = glm::acos(dotProduct);
                auto reboundAngle = 3.14f - incidenceAngle;

                boxCollider.rigidbody->velocity = wallNormal * glm::cos(reboundAngle) * glm::length(boxCollider.rigidbody->velocity);
            }



            return data.isCollisoned;
        }

        bool Collision::DoCollision(const BoxCollider& boxCollider, const CircleCollider& circleCollider)
        {
            return Collision::DoCollision(circleCollider, boxCollider);
        }
    }
}