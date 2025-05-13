#pragma once

#include <vector>

#include "AnimationFrame.h"
#include "../Render/Texture2DContainer.h"

namespace Learning2DEngine
{
	namespace Animator
	{
		class Animation
		{
		private:
			Render::Texture2DContainer* textureContainer;
			std::vector<AnimationFrame> frames;
			size_t currentIndex;
			float currentTime;
			bool isPlaying;
		public:
			float speed;
			bool isLoop;

			Animation(Render::Texture2DContainer* textureContainer, bool isLoop = false);
			Animation(Render::Texture2DContainer* textureContainer, size_t minFrameSize, bool isLoop = false);
			~Animation() = default;

			void Update();
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
			const AnimationFrame& GetCurrentFrame() const;

		};
	}
}