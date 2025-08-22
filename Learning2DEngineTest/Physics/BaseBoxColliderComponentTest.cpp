#include "CppUnitTest.h"

#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BaseBoxColliderComponent.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	using namespace System;

    namespace Physics
    {
        TEST_CLASS(BaseBoxColliderComponentTest)
        {
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
			TEST_METHOD(GetColliderCenter)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();
				auto boxCollider = gameObject->AddComponent<TestBoxColliderComponent>(glm::vec2(10.0f, 20.0f));
                Assert::IsTrue(boxCollider->GetColliderCenter() == glm::vec2(5.0f, 10.0f));

				boxCollider->colliderOffset = glm::vec2(2.0f, 3.0f);
				Assert::IsTrue(boxCollider->GetColliderCenter() == glm::vec2(7.0f, 13.0f));

				gameObject->transform.SetPosition(glm::vec2(10.0f, 15.0f));
                Assert::IsTrue(boxCollider->GetColliderCenter() == glm::vec2(17.0f, 28.0f));

				manager.DestroyAllGameObjects();
			}
        };
    }
}