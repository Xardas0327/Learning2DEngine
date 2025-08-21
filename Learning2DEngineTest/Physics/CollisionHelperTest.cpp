#include "CppUnitTest.h"

#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BaseCircleColliderComponent.h>
#include <Learning2DEngine/Physics/BaseBoxColliderComponent.h>
#include <Learning2DEngine/Physics/CollisionHelper.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
    using namespace System;

    namespace Physics
    {
        TEST_CLASS(CollisionHelperTest)
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

            class TestBoxColliderComponent : public BaseBoxColliderComponent
            {
            public:
                TestBoxColliderComponent(
                    System::GameObject* gameObject,
                    glm::vec2 size,
                    ColliderType type = ColliderType::DYNAMIC,
                    ColliderMode mode = ColliderMode::TRIGGER,
                    glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                    int32_t maskLayer = ~0)
                    : System::Component(gameObject), BaseBoxColliderComponent(gameObject, size, type, mode, offset, maskLayer)
                {
                }
            };
        public:
            TEST_METHOD(CheckCollisionBoxBoxNotCollide)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject1 = manager.CreateGameObject();
                auto boxCollider1 = gameObject1->AddComponent<TestBoxColliderComponent>(glm::vec2(5.0f, 10.0f));

                auto gameObject2 = manager.CreateGameObject(Transform(glm::vec2(50.0f, 50.0f)));
                auto boxCollider2 = gameObject2->AddComponent<TestBoxColliderComponent>(glm::vec2(10.0f, 5.0f));

				auto result = CollisionHelper::CheckCollision(*boxCollider1, *boxCollider2);
                Assert::IsTrue(!result.isCollided);

                manager.DestroyAllGameObjects();
            }

            TEST_METHOD(CheckCollisionBoxBoxCollide)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject1 = manager.CreateGameObject();
                auto boxCollider1 = gameObject1->AddComponent<TestBoxColliderComponent>(glm::vec2(5.0f, 10.0f));

                auto gameObject2 = manager.CreateGameObject(Transform(glm::vec2(5.0f, 5.0f)));
                auto boxCollider2 = gameObject2->AddComponent<TestBoxColliderComponent>(glm::vec2(10.0f, 5.0f));

                auto result = CollisionHelper::CheckCollision(*boxCollider1, *boxCollider2);
                Assert::IsTrue(result.isCollided);
                Assert::IsTrue(result.edge1 == glm::vec2(5.0f, 7.5f));
                Assert::IsTrue(result.edge2 == glm::vec2(5.0f, 5.0f));

                manager.DestroyAllGameObjects();
            }

            TEST_METHOD(CheckCollisionCircleCircleNotCollide)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject1 = manager.CreateGameObject();
                auto circleCollider1 = gameObject1->AddComponent<TestCircleColliderComponent>(5.0f);

                auto gameObject2 = manager.CreateGameObject(Transform(glm::vec2(50.0f, 50.0f)));
                auto circleCollider2 = gameObject2->AddComponent<TestCircleColliderComponent>(5.0f);

                auto result = CollisionHelper::CheckCollision(*circleCollider1, *circleCollider2);
                Assert::IsTrue(!result.isCollided);

                manager.DestroyAllGameObjects();
			}

            TEST_METHOD(CheckCollisionCircleCircleCollide)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject1 = manager.CreateGameObject();
                auto circleCollider1 = gameObject1->AddComponent<TestCircleColliderComponent>(15.0f);

                auto gameObject2 = manager.CreateGameObject(Transform(glm::vec2(20.0f, 15.0f)));
                auto circleCollider2 = gameObject2->AddComponent<TestCircleColliderComponent>(15.0f);

                auto result = CollisionHelper::CheckCollision(*circleCollider1, *circleCollider2);
                Assert::IsTrue(result.isCollided);
                Assert::IsTrue(result.edge1 == glm::vec2(27.0f, 24.0f));
                Assert::IsTrue(result.edge2 == glm::vec2(23.0f, 21.0f));

                manager.DestroyAllGameObjects();
            }

            TEST_METHOD(CheckCollisionCircleBoxNotCollide)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject1 = manager.CreateGameObject();
                auto circleCollider = gameObject1->AddComponent<TestCircleColliderComponent>(5.0f);

                auto gameObject2 = manager.CreateGameObject(Transform(glm::vec2(50.0f, 50.0f)));
                auto boxCollider = gameObject2->AddComponent<TestBoxColliderComponent>(glm::vec2(10.0f, 5.0f));

                auto result = CollisionHelper::CheckCollision(*circleCollider, *boxCollider);
                Assert::IsTrue(!result.isCollided);

                manager.DestroyAllGameObjects();
            }

            TEST_METHOD(CheckCollisionCircleBoxCollide)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject1 = manager.CreateGameObject();
                auto circleCollider = gameObject1->AddComponent<TestCircleColliderComponent>(15.0f);

                auto gameObject2 = manager.CreateGameObject(Transform(glm::vec2(20.0f, 20.0f)));
                auto boxCollider = gameObject2->AddComponent<TestBoxColliderComponent>(glm::vec2(10.0f, 5.0f));

                auto result = CollisionHelper::CheckCollision(*circleCollider, *boxCollider);
                Assert::IsTrue(result.isCollided);
                Assert::IsTrue(result.edge1 == glm::vec2(27.0f, 24.0f));
                Assert::IsTrue(result.edge2 == glm::vec2(20.0f, 20.0f));

                manager.DestroyAllGameObjects();
            }

            TEST_METHOD(CheckCollisionBoxCircleNotCollide)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject1 = manager.CreateGameObject();
                auto circleCollider = gameObject1->AddComponent<TestCircleColliderComponent>(5.0f);

                auto gameObject2 = manager.CreateGameObject(Transform(glm::vec2(50.0f, 50.0f)));
                auto boxCollider = gameObject2->AddComponent<TestBoxColliderComponent>(glm::vec2(10.0f, 5.0f));

                auto result = CollisionHelper::CheckCollision(*boxCollider, *circleCollider);
                Assert::IsTrue(!result.isCollided);

                manager.DestroyAllGameObjects();
            }

            TEST_METHOD(CheckCollisionBoxCircleCollide)
            {
                auto& manager = GameObjectManager::GetInstance();

                auto gameObject1 = manager.CreateGameObject();
                auto circleCollider = gameObject1->AddComponent<TestCircleColliderComponent>(15.0f);

                auto gameObject2 = manager.CreateGameObject(Transform(glm::vec2(20.0f, 20.0f)));
                auto boxCollider = gameObject2->AddComponent<TestBoxColliderComponent>(glm::vec2(10.0f, 5.0f));

                auto result = CollisionHelper::CheckCollision(*boxCollider, *circleCollider);
                Assert::IsTrue(result.isCollided);
                Assert::IsTrue(result.edge1 == glm::vec2(20.0f, 20.0f));
                Assert::IsTrue(result.edge2 == glm::vec2(27.0f, 24.0f));

                manager.DestroyAllGameObjects();
            }
        };
    }
}