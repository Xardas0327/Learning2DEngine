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
    renderer = gameObject->AddComponent<SpriteRenderComponent>(
        RendererMode::RENDER,
        ResourceManager::GetInstance().GetTexture(textureId),
        0,
        color
    );
}