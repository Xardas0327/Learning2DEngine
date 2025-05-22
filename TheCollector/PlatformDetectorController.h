#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/EventSystem/EventHandler.h>

class PlatformDetectorController : public Learning2DEngine::Physics::BoxColliderComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    PlatformDetectorController(Learning2DEngine::System::GameObject* gameObject, glm::vec2 size, glm::vec2 offset);

    void Destroy() override;
    void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;

public:
    Learning2DEngine::EventSystem::EventHandler<> eventhandler;
};