#pragma once

#include <string>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>

const glm::vec3 FOOD_COLOR = glm::vec3(0.75f, 0.0f, 0.0f);

class FoodController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    const std::string textureId;
    const glm::vec3 color;
    Learning2DEngine::Render::SpriteRenderer* renderer;

    FoodController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId);
    void Init() override;
    // TODO: This function have to be removed
    void Destroy() override {};
public:
    void Draw();
};