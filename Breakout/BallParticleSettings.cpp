#include "BallParticleSettings.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/Debug/Log.h>
#include <Learning2DEngine/System/Random.h>

using namespace Learning2DEngine::ParticleSimulator;
using namespace Learning2DEngine::System;

BallParticleSettings::BallParticleSettings(glm::vec2 positionOffset, glm::vec2 scale, float lifeTime, float speed)
	: ballController(nullptr), positionOffset(positionOffset), scale(scale), lifeTime(lifeTime), speed(speed)
{

}
void BallParticleSettings::Init(const GameObject& gameObject)
{
	ballController = gameObject.GetComponent<BallController>();

	if (ballController == nullptr)
	{
		LOG_ERROR("BallParticleSettings: BallController is missing.");
	}
}
void BallParticleSettings::SpawnParticle(Particle& particle, const GameObject& gameObject)
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

void BallParticleSettings::UpdateParticle(Particle& particle, const GameObject& gameObject)
{
	particle.transform.position -= particle.velocity * particle.speed * Game::GetDeltaTime();
	particle.color.a -= Game::GetDeltaTime() * 2.5f;
}