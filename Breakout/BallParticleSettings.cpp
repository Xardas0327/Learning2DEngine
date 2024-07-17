#include "BallParticleSettings.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/Log.h>
#include <Learning2DEngine/System/Random.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

BallParticleSettings::BallParticleSettings(glm::vec2 positionOffset, glm::vec2 scale, float lifeTime, float speed)
	: ballController(nullptr), positionOffset(positionOffset), scale(scale), lifeTime(lifeTime), speed(speed)
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
	float random = Random::GetNumber(-5, 6);
	float rColor = Random::GetNumber(0.5f, 0.6f);
	particle.transform.position = gameObject.transform.position + random + positionOffset;
	particle.transform.scale = scale;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.lifeTime = lifeTime;
	particle.velocity = ballController->velocity;
	particle.speed = speed;
}

void BallParticleSettings::UpdateParticle(Learning2DEngine::Render::Particle& particle, const Learning2DEngine::System::GameObject& gameObject)
{
	particle.transform.position -= particle.velocity * particle.speed * Game::GetDeltaTime();
	particle.color.a -= Game::GetDeltaTime() * 2.5f;
}