#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Learning2DEngine/Render/Texture2D.h"
#include "Learning2DEngine/System/Transform.h"
#include "sprite_renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    Learning2DEngine::System::Transform transform;
    glm::vec2   Velocity;
    glm::vec3   Color;
    bool        IsSolid;
    bool        Destroyed;
    // render state
    Learning2DEngine::Render::Texture2D   Sprite;
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Learning2DEngine::Render::Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // draw sprite
    virtual void Draw(SpriteRenderer& renderer);
};