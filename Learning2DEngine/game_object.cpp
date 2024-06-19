#include "game_object.h"

using namespace Learning2DEngine::Render;


GameObject::GameObject()
    : transform(),  Velocity(0.0f), Color(1.0f), Sprite(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : transform(pos, size), Velocity(velocity), Color(color), Sprite(sprite), IsSolid(false), Destroyed(false) { }

void GameObject::Draw(SpriteRenderer& renderer)
{
    renderer.DrawSprite(this->Sprite, this->transform.position, this->transform.size, this->transform.rotation, this->Color);
}