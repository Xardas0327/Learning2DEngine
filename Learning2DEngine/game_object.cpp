#include "game_object.h"

using namespace Learning2DEngine::Render;

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, const Learning2DEngine::Render::SpriteRenderer& spriteRenderer, glm::vec2 velocity)
    : transform(pos, size), Velocity(velocity), renderer(spriteRenderer), IsSolid(false), Destroyed(false) 
{
    renderer.Init();
}

GameObject::~GameObject()
{
    renderer.Terminate();
}

void GameObject::Draw()
{
    renderer.Draw(transform);
}