#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>

const glm::vec2 PLAYER_SIZE(50.0f, 50.0f);

class PlayerController : public virtual Learning2DEngine::System::UpdaterComponent,
                        //this is not the player, this is, that the player is on the ground tracker.
                        public virtual Learning2DEngine::Physics::BoxColliderComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    bool onGround;
    Learning2DEngine::Physics::Rigidbody* rigidbody;

    PlayerController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Destroy() override;
    void Update() override;
    void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;
};