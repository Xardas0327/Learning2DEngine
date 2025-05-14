#include "Animation.h"

#include <cmath>

#include "../System/Game.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

namespace Learning2DEngine
{
	namespace Animator
	{
		Animation::Animation(Texture2DContainer* textureContainer, bool isLoop)
			: textureContainer(textureContainer), frames(), currentIndex(0), currentTime(0.0f), isPlaying(false),
			speed(1.0f), isLoop(isLoop)
		{
			// there are less than 2 frames, it is useles
			frames.reserve(2);
		}

		Animation::Animation(Texture2DContainer* textureContainer, size_t minFrameSize, bool isLoop)
			: textureContainer(textureContainer), frames(), currentIndex(0), currentTime(0.0f), isPlaying(false),
			speed(1.0f), isLoop(isLoop)
		{
			frames.reserve(minFrameSize);
		}

		void Animation::Update()
		{
			if (!isPlaying)
				return;

			bool isForward = speed >= 0.0f;
			float currentSpeed = fabs(speed);

			currentTime -= currentSpeed * Game::GetDeltaTime();

			if (currentTime > 0.0f)
				return;

			if (isForward)
			{
				if (currentIndex == frames.size() - 1)
				{
					if (isLoop)
						currentIndex = 0;
					else
					{
						isPlaying = false;
						return;
					}
				}
				else
					++currentIndex;
			}
			else
			{
				if (currentIndex == 0)
				{
					if (isLoop)
						currentIndex = frames.size() - 1;
					else
					{
						isPlaying = false;
						return;
					}
				}
				else
					--currentIndex;
			}

			currentTime = frames[currentIndex].time;
			textureContainer->SetTexture(*frames[currentIndex].texture);
		}

		void Animation::Play(bool reset)
		{
			if(reset)
				currentIndex = 0;

			currentTime = frames[currentIndex].time;
			textureContainer->SetTexture(*frames[currentIndex].texture);
			isPlaying = true;
		}

		void Animation::Stop()
		{
			isPlaying = false;
		}

		void Animation::Add(const AnimationFrame& frame)
		{
			frames.push_back(frame);
		}

		void Animation::Add(AnimationFrame&& frame)
		{
			frames.push_back(std::move(frame));
		}

		void Animation::Remove(size_t index)
		{
			frames.erase(frames.begin() + index);
		}

		void Animation::Clear()
		{
			frames.clear();
		}

		AnimationFrame& Animation::operator[](size_t index)
		{
			return frames[index];
		}

		const AnimationFrame& Animation::operator[](size_t index) const
		{
			return frames[index];
		}

		void Animation::JumpToFrame(size_t index, float time)
		{
			currentIndex = index;
			currentTime = time < 0.0f ? frames[currentIndex].time : time;
		}

		const AnimationFrame& Animation::GetCurrentFrame() const
		{
			return frames[currentIndex];
		}
	}
}