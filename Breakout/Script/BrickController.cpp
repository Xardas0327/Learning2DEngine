#include "BrickController.h"

#include <Learning2DEngine/Render/Texture2D.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

BrickController::BrickController(GameObject* gameObject, size_t x, size_t y, bool isSolid)
    : Component(gameObject), mapCoorX(x), mapCoorY(y), isSolid(isSolid), collider(nullptr), renderer(nullptr)
{

}

void BrickController::Init()
{
    collider = gameObject->AddComponent<BoxColliderComponent>(
        gameObject->transform.GetScale(), ColliderType::KINEMATIC, ColliderMode::COLLIDER, glm::vec2(0.0f, 0.0f), 0b1
    );
    renderer = gameObject->AddComponent<SpriteRenderComponent>(RendererMode::RENDER);
}