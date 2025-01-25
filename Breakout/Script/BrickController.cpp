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
    collider = gameObject->AddComponent<BoxColliderComponent, bool, glm::vec2>(true, gameObject->transform.scale);
    renderer = gameObject->AddComponent<SpriteRenderer>();
}