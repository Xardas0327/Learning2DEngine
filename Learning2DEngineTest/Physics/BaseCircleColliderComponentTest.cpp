#include "CppUnitTest.h"

#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BaseCircleColliderComponent.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
    using namespace System;

    namespace Physics
    {
        TEST_CLASS(BaseCircleColliderComponentTest)
        {
            class TestCircleColliderComponent : public BaseCircleColliderComponent
            {
            public:
                TestCircleColliderComponent(
                    System::GameObject* gameObject,
                    float radius,
                    ColliderType type = ColliderType::DYNAMIC,
                    ColliderMode mode = ColliderMode::TRIGGER,
                    glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                    int32_t maskLayer = ~0)
                    : System::Component(gameObject), BaseCircleColliderComponent(gameObject, radius, type, mode, offset, maskLayer)
                {
                }
            };
        public:
            TEST_METHOD(GetColliderCenter)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject = manager.CreateGameObject();
                auto circleCollider = gameObject->AddComponent<TestCircleColliderComponent>(5.0f);
                Assert::IsTrue(circleCollider->GetColliderCenter() == glm::vec2(5.0f, 5.0f));

                circleCollider->colliderOffset = glm::vec2(2.0f, 3.0f);
                Assert::IsTrue(circleCollider->GetColliderCenter() == glm::vec2(7.0f, 8.0f));

                gameObject->transform.SetPosition(glm::vec2(10.0f, 15.0f));
                Assert::IsTrue(circleCollider->GetColliderCenter() == glm::vec2(17.0f, 23.0f));

                manager.DestroyAllGameObjects();
            }
        };
    }
}