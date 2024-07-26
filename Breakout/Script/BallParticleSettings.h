#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/ParticleSimulator/ParticleSettings.h>
#include <Learning2DEngine/Physics/Rigidbody.h>

class BallParticleSettings : public virtual Learning2DEngine::ParticleSimulator::ParticleSettings
{
private:
	const Learning2DEngine::Physics::Rigidbody* rigidbody;
public:
	glm::vec2 positionOffset;
	glm::vec2 scale;
	float lifeTime;
	float speed;

	BallParticleSettings(glm::vec2 positionOffset = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f), float lifeTime = 1.0f, float speed = 1.0f);
	void Init(const  Learning2DEngine::System::GameObject& gameObject) override;
	void SpawnParticle(Learning2DEngine::ParticleSimulator::Particle& particle, const Learning2DEngine::System::GameObject& gameObject) override;
	void UpdateParticle(Learning2DEngine::ParticleSimulator::Particle& particle, const Learning2DEngine::System::GameObject& gameObject) override;
};