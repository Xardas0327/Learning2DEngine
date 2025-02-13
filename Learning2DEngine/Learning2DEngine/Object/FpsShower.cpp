#include "FpsShower.h"

#include <GLFW/glfw3.h>
#include <string>

namespace Learning2DEngine
{
	using namespace Render;
	using namespace System;
	using namespace UI;

	namespace Object
	{
		FpsShower::FpsShower(
			GameObject* gameObject,
			const Resolution& cameraResolution,
			const FontSizePair& fontSizePair,
			int layer,
			glm::vec3 color)
			: UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), Component(gameObject),
			Text2DLateRenderer(gameObject, cameraResolution, fontSizePair, layer, color),
			LateRendererComponent(gameObject, layer),  BaseRendererComponent(gameObject, layer), previousTime(0.0f), fps(0)
		{

		}

		void FpsShower::Init()
		{
			UpdaterComponent::Init();
			Text2DLateRenderer::Init();
			previousTime = glfwGetTime();
			text = "FPS: " + std::to_string(fps);
		}

		void FpsShower::Destroy()
		{
			UpdaterComponent::Destroy();
			Text2DLateRenderer::Destroy();
		}

		void FpsShower::Update()
		{
			++fps;
			// The Game::deltaTime is not good here, because it is multiplided with Game::timeScale
			float currentTime = glfwGetTime();
			if (currentTime - previousTime > 1.0f)
			{
				text = "FPS: " + std::to_string(fps);
				fps = 0;
				previousTime = currentTime;
			}
		}

		FpsShower* FpsShower::CreateFpsShowerObject(
			const System::Transform& transform,
			const Resolution& cameraResolution,
			const FontSizePair& fontSizePair,
			int layer,
			glm::vec3 color)
		{
			auto gameObject = new GameObject(transform);
			return gameObject->AddComponent<FpsShower, const Resolution&, const FontSizePair&, int, glm::vec3>(
				cameraResolution,
				fontSizePair,
				layer,
				color
			);
		}
	}
}