#pragma once

#include <glm/glm.hpp>

#include "Collider.h";
#include "BoxCollider.h";
#include "CircleCollider.h"
#include "BaseBoxColliderComponent.h"
#include "BaseCircleColliderComponent.h"
#include "CollisionData.h"


namespace Learning2DEngine
{
    namespace Physics
    {
        class CollisionChecker final
        {
        private:
            CollisionChecker() {}

            // TODO: This function have to be removed
            static glm::vec2 GetEdge(const BoxCollider& boxCollider, glm::vec2 distanceBetweenCenters);
            // TODO: This function have to be removed
            static glm::vec2 GetEdge(const CircleCollider& circleCollider, glm::vec2 distanceBetweenCenters);

            static glm::vec2 GetEdge(const BaseBoxColliderComponent& boxCollider, glm::vec2 distanceBetweenCenters);
            static glm::vec2 GetEdge(const BaseCircleColliderComponent& circleCollider, glm::vec2 distanceBetweenCenters);
        public:
            // TODO: This function have to be removed
            static CollisionData CheckCollision(const BoxCollider& collider1, const BoxCollider& collider2);
            // TODO: This function have to be removed
            static CollisionData CheckCollision(const CircleCollider& collider1, const CircleCollider& collider2);
            // TODO: This function have to be removed
            static CollisionData CheckCollision(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
            // TODO: This function have to be removed
            static CollisionData CheckCollision(const CircleCollider& circleCollider, const BaseBoxColliderComponent& boxCollider);

            static CollisionData CheckCollision(const BaseBoxColliderComponent& collider1, const BaseBoxColliderComponent& collider2);
            static CollisionData CheckCollision(const BaseCircleColliderComponent& collider1, const BaseCircleColliderComponent& collider2);
            static CollisionData CheckCollision(const BaseCircleColliderComponent& circleCollider, const BaseBoxColliderComponent& boxCollider);
            static CollisionData CheckCollision(const BaseBoxColliderComponent& boxCollider, const BaseCircleColliderComponent& circleCollider);
        };
    }
}