#pragma once

#include <glm/glm.hpp>

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

            static CollisionData IsCollisoned(const BoxCollider& collider1, const BoxCollider& collider2);
            static CollisionData IsCollisoned(const CircleCollider& collider1, const CircleCollider& collider2);
            static CollisionData IsCollisoned(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
        public:
            static bool DoCollision(const BoxCollider& collider1, const BoxCollider& collider2);
            static bool DoCollision(const CircleCollider& collider1, const CircleCollider& collider2);
            static bool DoCollision(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
            static bool DoCollision(const BoxCollider& boxCollider, const CircleCollider& circleCollider);
        };
    }
}