#pragma once

#include "BoxCollider.h";
#include "CircleCollider.h"


namespace Learning2DEngine
{
    namespace Physics
    {
        class Collision final
        {
        private:
            Collision() {}

            static bool IsCollisoned(const BoxCollider& collider1, const BoxCollider& collider2);
            static bool IsCollisoned(const CircleCollider& collider1, const CircleCollider& collider2);
            static bool IsCollisoned(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
        public:
            static void CheckCollision(const BoxCollider& collider1, const BoxCollider& collider2);
            static void CheckCollision(const CircleCollider& collider1, const CircleCollider& collider2);
            static void CheckCollision(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
            static void CheckCollision(const BoxCollider& boxCollider, const CircleCollider& circleCollider)
            {
                Collision::CheckCollision(circleCollider, boxCollider);
            }
        };
    }
}