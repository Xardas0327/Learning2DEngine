#pragma once

#include <glm/glm.hpp>

#include "Learning2DEngine/Render/IParticleSettings.h"

#include "BallController.h"

class BallParticleSettings : public virtual Learning2DEngine::Render::IParticleSettings
{
private:
	const BallController* ballController;
public:
	glm::vec2 positionOffset;

	BallParticleSettings(glm::vec2 positionOffset = glm::vec2(0.0f));
	void Init(const  Learning2DEngine::System::GameObject& gameObject) override;
	void SpawnParticle(Learning2DEngine::Render::Particle& particle, const Learning2DEngine::System::GameObject& gameObject) override;
	void UpdateParticle(Learning2DEngine::Render::Particle& particle, const Learning2DEngine::System::GameObject& gameObject) override;
};