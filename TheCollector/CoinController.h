#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Component.h>

const glm::vec2 COIN_SIZE(50.0f, 50.0f);
const std::vector<std::string> COIN_TEXTURE_IDS = {
	"Coin1", "Coin2", "Coin3", "Coin4", "Coin5", "Coin6",
	"Coin7", "Coin8", "Coin9", "Coin10", "Coin11", "Coin12",
};

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