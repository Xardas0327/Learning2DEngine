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
			static const char* GetSimpleSpriteVertexShader();
			static const char* GetSimpleSpriteFragmentShader();

			//Old Sprite (Multi instancing support)

			static const std::string OLD_SPRITE_SHADER_NAME;
			static const char* GetOldSpriteVertexShader();
			static const char* GetOldSpriteFragmentShader();

			//Sprite (Multi instancing support)

			static const std::string SPRITE_SHADER_NAME;
			static const char* GetSpriteVertexShader();
			static const char* GetSpriteFragmentShader();

			//Text2D

			static const std::string TEXT2D_SHADER_NAME;
			static const char* GetText2DVertexShader();
			static const char* GetText2DFragmentShader();

			//PostProcessEffect

			static const std::string DEFAULT_POSTPROCESS_EFFECT_NAME;
			static const char* GetDefaultPostprocessVertexShader();
			static const char* GetDefaultPostprocessFragmentShader();
		};
	}
}