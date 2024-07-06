#include "BallParticleSettings.h"

#include "Learning2DEngine/System/Game.h"
#include "Learning2DEngine/System/Log.h"
#include "Learning2DEngine/System/Random.h"

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

BallParticleSettings::BallParticleSettings(glm::vec2 positionOffset)
	: ballController(nullptr), positionOffset(positionOffset)
{

}
void BallParticleSettings::Init(const Learning2DEngine::System::GameObject& gameObject)
{
	ballController = gameObject.GetComponent<BallController>();

	if (ballController == nullptr)
	{
		LOG_ERROR("BallParticleSettings: BallController is missing.");
	}
}
void BallParticleSettings::SpawnParticle(Learning2DEngine::Render::Particle& particle, const Learning2DEngine::System::GameObject& gameObject)
{
	float random = (Random::GetNumber(-50, 50)) / 10.0f;
	float rColor = 0.5f + (Random::GetNumber(0, 100) / 100.0f);
	particle.transform.position = gameObject.transform.position + random + positionOffset;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.lifeTime = 1.0f;
	particle.velocity = ballController->velocity;
	particle.speed = 0.1f;
}

void BallParticleSettings::UpdateParticle(Learning2DEngine::Render::Particle& particle, const Learning2DEngine::System::GameObject& gameObject)
{
	particle.transform.position -= particle.velocity * particle.speed * Game::GetDeltaTime();
	particle.color.a -= Game::GetDeltaTime() * 2.5f;
}