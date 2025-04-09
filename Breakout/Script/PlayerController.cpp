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
	renderer = gameObject->AddComponent<SpriteRenderComponent, const Texture2D&>(
		ResourceManager::GetInstance().GetTexture(textureId)
	);
	collider = gameObject->AddComponent<BoxColliderComponent, glm::vec2, bool, glm::vec2, int32_t>(glm::vec2(0.0f, 0.0f), false, glm::vec2(0.0f, 0.0f), 0b110);

	Reset();
}

void PlayerController::Reset()
{
	const Resolution resolution = Game::mainCamera.GetResolution();

	gameObject->transform.scale = PLAYER_SIZE;
	gameObject->transform.position = glm::vec2(resolution.GetWidth() / 2.0f - PLAYER_SIZE.x / 2.0f, resolution.GetHeight() - PLAYER_SIZE.y);

	collider->colliderSize = PLAYER_SIZE;
	renderer->data.color = glm::vec4(1.0f);
}