#include "FoodController.h"

#include <Learning2DEngine/System/ResourceManager.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

FoodController::FoodController(GameObject* gameObject, const std::string& textureId)
	: Component(gameObject), textureId(textureId), color(FOOD_COLOR), renderer(nullptr)
{

}

void FoodController::Init()
{
    renderer = gameObject->AddComponent<SpriteRenderer, const Texture2D&, glm::vec3>(
        ResourceManager::GetInstance().GetTexture(textureId),
        color
    );
}

void FoodController::Draw()
{
    renderer->Draw();
}