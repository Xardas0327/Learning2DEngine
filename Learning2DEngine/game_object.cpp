#include "game_object.h"

using namespace Learning2DEngine::Render;

OldGameObject::OldGameObject(glm::vec2 pos, glm::vec2 size, const Learning2DEngine::Render::OldSpriteRenderer& spriteRenderer, glm::vec2 velocity)
    : transform(pos, size), Velocity(velocity), renderer(spriteRenderer), IsSolid(false), Destroyed(false) 
{
    renderer.Init();
}

OldGameObject::~OldGameObject()
{
    renderer.Destroy();
}

void OldGameObject::Draw()
{
    renderer.Draw(transform);
}