#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Component.h>

const glm::vec2 COIN_SIZE(8.0f, 8.0f);
const std::string COIN_TEXTURE_ID = "Coin";
const int COIN_ANIMATION_NUMBER = 12;

class CoinController : public Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
	float speed;

	CoinController(Learning2DEngine::System::GameObject* gameObject, float speed = 1.0f);

	void Init() override;

public:
	static CoinController* Create(glm::vec2 position, float speed = 1.0f);
};