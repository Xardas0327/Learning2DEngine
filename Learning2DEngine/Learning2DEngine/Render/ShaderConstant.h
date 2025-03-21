#pragma once

#include <string>

namespace Learning2DEngine
{
	namespace Render
	{
		class ShaderConstant final
		{
		private:
			ShaderConstant() {}
		public:
			//Simple Sprite

			static const std::string SIMPLE_SPRITE_SHADER_NAME;
			static const char* const SIMPLE_SPRITE_VERTEX_SHADER;
			static const char* const SIMPLE_SPRITE_FRAGMENT_SHADER;

			//Sprite (Multi instancing support)

			static const std::string SPRITE_SHADER_NAME;
			static const char* const SPRITE_VERTEX_SHADER;
			static const char* const SPRITE_FRAGMENT_SHADER;

			//Text2D

			static const std::string TEXT2D_SHADER_NAME;
			static const char* const TEXT2D_VERTEX_SHADER;
			static const char* const TEXT2D_FRAGMENT_SHADER;

			//PostProcessEffect

			static const std::string DEFAULT_POSTPROCESS_EFFECT_NAME;
			static const char* const DEFAULT_POSTPROCESS_EFFECT_VERTEX_SHADER;
			static const char* const DEFAULT_POSTPROCESS_EFFECT_FRAGMENT_SHADER;
		};
	}
}