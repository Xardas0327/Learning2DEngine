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
			static void Init();
		public:
			static bool isInited;

			/// <summary>
			/// Return a random int within [minInclusive..maxExclusive)
			/// </summary>
			/// <param name="minInclusive"></param>
			/// <param name="maxExclusive"></param>
			/// <returns></returns>
			static int GetNumber(int minInclusive, int maxExclusive);

			/// <summary>
			/// Return a random float within [minInclusive..maxInclusive]
			/// </summary>
			/// <param name="minInclusive"></param>
			/// <param name="maxInclusive"></param>
			/// <returns></returns>
			static float GetNumber(float minInclusive, float maxInclusive);
		};
	}
}