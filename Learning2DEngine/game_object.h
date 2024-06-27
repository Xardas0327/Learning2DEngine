#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Learning2DEngine/Render/OldSpriteRenderer.h"
#include "Learning2DEngine/System/Transform.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class OldGameObject
{
public:
    // object state
    Learning2DEngine::System::Transform transform;
    glm::vec2   Velocity;
    bool        IsSolid;
    bool        Destroyed;
    Learning2DEngine::Render::OldSpriteRenderer renderer;
    OldGameObject(glm::vec2 pos, glm::vec2 size, const Learning2DEngine::Render::OldSpriteRenderer& spriteRenderer, glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    ~OldGameObject();
    // draw sprite
    virtual void Draw();
};