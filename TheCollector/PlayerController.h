#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Render/Texture2D.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

const glm::vec2 PLAYER_SIZE(50.0f, 50.0f);
const std::string PLAYER_RIGHT_TEXTURE_ID = "KnightRight";
const std::string PLAYER_LEFT_TEXTURE_ID = "KnightLeft";

class PlayerController : public virtual Learning2DEngine::System::UpdaterComponent,
                        //this is not the player, this is, that the player is on the ground tracker.
                        public virtual Learning2DEngine::Physics::BoxColliderComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    bool onGround;
    Learning2DEngine::Physics::Rigidbody* rigidbody;
    Learning2DEngine::Render::SpriteRenderComponent* render;
    Learning2DEngine::Render::Texture2D rightSide;
    Learning2DEngine::Render::Texture2D leftSide;

    PlayerController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Destroy() override;
    void Update() override;
    void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;
};