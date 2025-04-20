#include "BrickController.h"

#include <Learning2DEngine/Render/Texture2D.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

BrickController::BrickController(GameObject* gameObject, int x, int y, bool isSolid)
    : Component(gameObject), mapCoorX(x), mapCoorY(y), isSolid(isSolid), collider(nullptr), renderer(nullptr)
{

}

void BrickController::Init()
{
    collider = gameObject->AddComponent<BoxColliderComponent, glm::vec2, ColliderType, ColliderMode, glm::vec2, int32_t>(
        gameObject->transform.GetScale(), ColliderType::KINEMATIC, ColliderMode::TRIGGER, glm::vec2(0.0f, 0.0f), 0b1
    );
    renderer = gameObject->AddComponent<SpriteRenderComponent>();
}