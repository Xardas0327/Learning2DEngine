#pragma once

#include <vector>

#include "AnimationFrame.h"
#include "../Render/Texture2DContainer.h"
#include "../System/LateUpdaterComponent.h"
#include "../System/GameObject.h"

namespace Learning2DEngine
{
	namespace Animator
	{
		class AnimationController : public virtual Learning2DEngine::System::LateUpdaterComponent
		{
			friend class Learning2DEngine::System::GameObject;
		protected:
			Render::Texture2DContainer* textureContainer;
			std::vector<AnimationFrame> frames;
			size_t currentIndex;
			float currentTime;
			bool isPlaying;

			AnimationController(System::GameObject* gameObject, Render::Texture2DContainer* textureContainer, bool isLoop = false);
			AnimationController(System::GameObject* gameObject, Render::Texture2DContainer* textureContainer, size_t minFrameSize, bool isLoop = false);

			void LateUpdate() override;
		public:
			float speed;
			bool isLoop;

			void Play(bool reset = false);
			void Stop();
			void Add(const AnimationFrame& frame);
			void Add(AnimationFrame&& frame);
			void Remove(size_t index);
			void Clear();
			AnimationFrame& operator[](size_t index);
			const AnimationFrame& operator[](size_t index) const;

			//If the time is less then 0.0f, it will use the index frame time.
			void JumpToFrame(size_t index, float time = -1.0f);

			const inline AnimationFrame& GetCurrentFrame() const
			{
				return frames[currentIndex];
			}

			inline size_t GetCurrentIndex() const
			{
				return currentIndex;
			}

			inline float GetCurrentTime() const
			{
				return currentTime;
			}

		};
	}
}