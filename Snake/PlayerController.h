#pragma once

#include <string>
#include <list>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>

const unsigned int INIT_SNAKE_LENGTH = 3;
const glm::vec3 PLAYER_COLOR = glm::vec3(0.0f, 0.75f, 0.0f);

class PlayerController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    const std::string textureId;
    const int initSize;
    const glm::vec3 color;
    std::list<Learning2DEngine::Render::SpriteRenderer*> snake;

    PlayerController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId);
    // TODO: This function have to be removed
    void Init() override {};
    void Destroy() override;
    Learning2DEngine::Render::SpriteRenderer* CreateNewSnakeUnit(glm::vec2 position, glm::vec2 scale);
public:
    void Draw();
    void Regenerate(glm::ivec2 unitSize);
    void IncreaseSize(glm::vec2 headPostion, glm::ivec2 unitSize);
    bool IsInSnake(glm::vec2 position);
    void Move(glm::vec2 headPostion);

    inline unsigned int GetSize()
    {
        return snake.size();
    }

    inline glm::vec2 GetHeadPosition()
    {
        return snake.front()->gameObject->transform.position;
    }
};