#include "BackgroundController.h"

#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/RenderManager.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

BackgroundController::BackgroundController(GameObject* gameObject, const std::string& textureId, const Resolution& resolution)
    : Component(gameObject), renderer(nullptr), resolutionEventItem(this), textureId(textureId)
{
	gameObject->transform.SetScale(resolution.ToVec2());
}

void BackgroundController::Init()
{
    renderer = gameObject->AddComponent<SimpleSpriteRenderComponent>(
        RendererMode::RENDER,
        ResourceManager::GetInstance().GetTexture(textureId),
        -2
    );
    RenderManager::GetInstance().AddFramebufferSizeEvent(&resolutionEventItem);
}

void BackgroundController::Destroy()
{
    RenderManager::GetInstance().RemoveFramebufferSizeEvent(&resolutionEventItem);
}

void BackgroundController::RefreshResolution(const Resolution& resolution)
{
    gameObject->transform.SetScale(resolution.ToVec2());
}