#include "PlayerController.h"

#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Game.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

PlayerController::PlayerController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId)
	:Component(gameObject), textureId(textureId), renderer(nullptr), collider(nullptr)
{

}

void PlayerController::Init()
{
	renderer = gameObject->AddComponent<SpriteRenderer, const Texture2D&>(
		ResourceManager::GetInstance().GetTexture(textureId)
	);
	collider = gameObject->AddComponent<BoxCollider, glm::vec2>(glm::vec2(0.0f));

	Reset();
}

void PlayerController::Reset()
{
	const Resolution resolution = Game::GetCameraResolution();

	gameObject->transform.scale = PLAYER_SIZE;
	gameObject->transform.position = glm::vec2(resolution.GetWidth() / 2.0f - PLAYER_SIZE.x / 2.0f, resolution.GetHeight() - PLAYER_SIZE.y);

	collider->size = PLAYER_SIZE;
	renderer->color = glm::vec3(1.0f);
}