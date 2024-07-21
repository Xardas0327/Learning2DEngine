#pragma once

#include <glm/glm.hpp>

#include "Collider.h";
#include "BoxCollider.h";
#include "CircleCollider.h"
#include "CollisionData.h"


namespace Learning2DEngine
{
    namespace Physics
    {
        class Collision final
        {
        private:
            static const float PI;
            static const glm::vec2 VECTOR_UP;
            static const glm::vec2 VECTOR_DOWN;
            static const glm::vec2 VECTOR_LEFT;
            static const glm::vec2 VECTOR_RIGHT;

            enum Direction {
                UP,
                RIGHT,
                DOWN,
                LEFT
            };

            Collision() {}

            static glm::vec2 GetEdge(const BoxCollider& boxCollider, glm::vec2 distanceBetweenCenters);
            static glm::vec2 GetEdge(const CircleCollider& circleCollider, glm::vec2 distanceBetweenCenters);

            static void FixObjectAfterBoxCollision(const Collider* collider, glm::vec2 differenceVector);
            static void FixObjectAfterCircleCollision(const Collider* collider, glm::vec2 hitPoint, glm::vec2 differenceVector);
        public:
            static bool DoCollision(const BoxCollider& collider1, const BoxCollider& collider2);
            static bool DoCollision(const CircleCollider& collider1, const CircleCollider& collider2);
            static bool DoCollision(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
            static bool DoCollision(const BoxCollider& boxCollider, const CircleCollider& circleCollider);

            /// <summary>
            /// They are public for testing only
            /// </summary>
            static CollisionData IsCollisoned(const BoxCollider& collider1, const BoxCollider& collider2);
            static CollisionData IsCollisoned(const CircleCollider& collider1, const CircleCollider& collider2);
            static CollisionData IsCollisoned(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
            /// --------
        };
    }
}