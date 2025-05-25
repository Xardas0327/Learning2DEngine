#include "BallParticleSettings.h"

#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/DebugTool/Log.h>
#include <Learning2DEngine/System/Random.h>

using namespace Learning2DEngine::ParticleSimulator;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

BallParticleSettings::BallParticleSettings(glm::vec2 positionOffset, glm::vec2 scale, float lifeTime, float speed)
	: rigidbody(nullptr), positionOffset(positionOffset), scale(scale), lifeTime(lifeTime), speed(speed)
{

}
void BallParticleSettings::Init(const GameObject& gameObject)
{
	rigidbody = gameObject.GetComponent<Rigidbody>();

	if (rigidbody == nullptr)
	{
		L2DE_LOG_ERROR("BallParticleSettings: Rigidbody is missing.");
	}
}
void BallParticleSettings::SpawnParticle(Particle& particle, const GameObject& gameObject)
{
	float random = static_cast<float>(Random::GetNumber(-5, 6));
	float rColor = Random::GetNumber(0.5f, 0.6f);
	particle.transform.SetPosition(
		gameObject.transform.GetPosition() + random + positionOffset
	);
	particle.transform.SetScale(scale);
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.lifeTime = lifeTime;
	particle.velocity = rigidbody->velocity;
	particle.speed = speed;
}

void BallParticleSettings::UpdateParticle(Particle& particle, const GameObject& gameObject)
{
	particle.transform.AddPosition(-particle.velocity * particle.speed * Time::GetDeltaTime());
	particle.color.a -= Time::GetDeltaTime() * 2.5f;
}