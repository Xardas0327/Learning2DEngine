#pragma once

#include "TiledMap.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class TiledMapLoader final
		{
		private:
			TiledMapLoader() = default;
		public:
			~TiledMapLoader() = default;

			static TiledMap LoadFromFile(const char* filePath);
		};
	}
}