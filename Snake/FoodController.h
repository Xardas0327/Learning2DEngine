#pragma once

#include <string>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SimpleSpriteRenderComponent.h>

const glm::vec4 FOOD_COLOR = glm::vec4(0.75f, 0.0f, 0.0f, 1.0f);

class FoodController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    const std::string textureId;
    const glm::vec4 color;
    Learning2DEngine::Render::SimpleSpriteRenderComponent* renderer;

    FoodController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId);
    void Init() override;
};