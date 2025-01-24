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
        class CollisionChecker final
        {
        private:
            CollisionChecker() {}

            static glm::vec2 GetEdge(const BoxCollider& boxCollider, glm::vec2 distanceBetweenCenters);
            static glm::vec2 GetEdge(const CircleCollider& circleCollider, glm::vec2 distanceBetweenCenters);
        public:
            static CollisionData CheckCollision(const BoxCollider& collider1, const BoxCollider& collider2);
            static CollisionData CheckCollision(const CircleCollider& collider1, const CircleCollider& collider2);
            static CollisionData CheckCollision(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
        };
    }
}