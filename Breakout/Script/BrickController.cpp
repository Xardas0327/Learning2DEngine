#include "BrickController.h"

#include <Learning2DEngine/Render/Texture2D.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

BrickController::BrickController(GameObject* gameObject, int x, int y, bool isSolid)
    : Component(gameObject), x(x), y(y), isSolid(isSolid), collider(nullptr), renderer(nullptr)
{

}

void BrickController::Init()
{
    collider = gameObject->AddComponent<Learning2DEngine::Physics::BoxCollider, glm::vec2>(gameObject->transform.scale);
    renderer = gameObject->AddComponent<SpriteRenderer>();
}