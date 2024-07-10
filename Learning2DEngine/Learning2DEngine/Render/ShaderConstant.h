#pragma once

#include <string>

namespace Learning2DEngine
{
	namespace Render
	{
		class ShaderConstant
		{
		private:
			ShaderConstant() {}
		public:
			//Sprite
			static const char* const SPRITE_VERTEX_SHADER_PATH;
			static const char* const SPRITE_FRAGMENT_SHADER_PATH;
			static const std::string SPRITE_SHADER_NAME;

			//Text2D
			static const char* const TEXT2D_VERTEX_SHADER_PATH;
			static const char* const TEXT2D_FRAGMENT_SHADER_PATH;
			static const std::string TEXT2D_SHADER_NAME;
		};
	}
}