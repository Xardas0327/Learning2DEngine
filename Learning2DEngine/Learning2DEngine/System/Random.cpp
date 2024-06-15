#include "Random.h"

#include <cstdlib>
#include <ctime>
#include <exception>
#include <string>

#include "Log.h"

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
				std::string errorMessage = "Random::GetNumber: The minInclusive ("+std::to_string(minInclusive)
					+ ") is not lesser than maxExclusive ("+ std::to_string(maxExclusive) + ").";
				LOG_ERROR(errorMessage);
				throw std::exception(errorMessage.c_str());
			}

			Init();
			int range = maxExclusive - minInclusive;

			return (rand() % range) + minInclusive;
		}

		float Random::GetNumber(float minInclusive, float maxInclusive)
		{
			if (minInclusive >= maxInclusive)
			{
				std::string errorMessage = "Random::GetNumber: The minInclusive (" + std::to_string(minInclusive) 
					+ ") is not lesser than maxInclusive (" + std::to_string(maxInclusive) + ").";
				LOG_ERROR(errorMessage);
				throw std::exception(errorMessage.c_str());
			}

			Init();

			//Between [0..1]
			float number = static_cast<float>(std::rand()) / RAND_MAX;

			float range = maxInclusive - minInclusive;

			return number * range + minInclusive;
		}
	}
}