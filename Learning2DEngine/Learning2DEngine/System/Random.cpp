#include "Random.h"

namespace Learning2DEngine
{
	namespace System
	{
		bool Random::isInited = false;

		void Random::Init()
		{
			if (isInited)
				return;

			std::srand(std::time(nullptr));

			isInited = true;
		}
		
		int Random::GetNumber(int minInclusive, int maxExclusive)
		{
			if (minInclusive >= maxExclusive)
			{
				const char* errorMessage = "Random::GetNumber: The minInclusive is bigger or equal than maxExclusive";
				LOG_ERROR(errorMessage);
				throw std::exception(errorMessage);
			}

			Init();
			int range = maxExclusive - minInclusive;

			return (rand() % range) + minInclusive;
		}

		float Random::GetNumber(float minInclusive, float maxInclusive)
		{
			if (minInclusive >= maxInclusive)
			{
				const char* errorMessage = "Random::GetNumber: The minInclusive is bigger or equal than maxInclusive";
				LOG_ERROR(errorMessage);
				throw std::exception(errorMessage);
			}

			Init();

			//Between [0..1]
			float number = static_cast<float>(std::rand()) / RAND_MAX;

			float range = maxInclusive - minInclusive;

			return number * range + minInclusive;
		}
	}
}