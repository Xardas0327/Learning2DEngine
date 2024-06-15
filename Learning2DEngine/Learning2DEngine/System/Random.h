#pragma once

#include <cstdlib>
#include <ctime>
#include <exception>

#include "Log.h"

namespace Learning2DEngine
{
	namespace System
	{
		/// <summary>
		/// It uses the rand() function from cstdlib.
		/// So, the randomness is depeden on RAND_MAX.
		/// If the isInited is false, the next GetNumber function will run the std::srand(std::time(nullptr)), before generation.
		/// </summary>
		class Random
		{
		private:

			Random() { }
			static void Init()
			{
				if (isInited)
					return;

				std::srand(std::time(nullptr));

				isInited = true;
			}
		public:
			static bool isInited;

			/// <summary>
			/// Return a random int within [minInclusive..maxExclusive)
			/// </summary>
			/// <param name="minInclusive"></param>
			/// <param name="maxExclusive"></param>
			/// <returns></returns>
			static int GetNumber(int minInclusive, int maxExclusive)
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

			/// <summary>
			/// Return a random float within [minInclusive..maxInclusive]
			/// </summary>
			/// <param name="minInclusive"></param>
			/// <param name="maxInclusive"></param>
			/// <returns></returns>
			static float GetNumber(float minInclusive, float maxInclusive)
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
		};

	}
}