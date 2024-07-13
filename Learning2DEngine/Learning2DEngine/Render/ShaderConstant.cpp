#include "ShaderConstant.h"

namespace Learning2DEngine
{
	namespace Render
	{
		//Sprite
		const char* const ShaderConstant::SPRITE_VERTEX_SHADER_PATH = "Learning2DEngine/Shader/Sprite.vs";
		const char* const ShaderConstant::SPRITE_FRAGMENT_SHADER_PATH = "Learning2DEngine/Shader/Sprite.fs";
		const std::string ShaderConstant::SPRITE_SHADER_NAME = "Learning2DEngine::Shader::Sprite";

		//Text2D
		const char* const ShaderConstant::TEXT2D_VERTEX_SHADER_PATH = "Learning2DEngine/Shader/Text2D.vs";
		const char* const ShaderConstant::TEXT2D_FRAGMENT_SHADER_PATH = "Learning2DEngine/Shader/Text2D.fs";
		const std::string ShaderConstant::TEXT2D_SHADER_NAME = "Learning2DEngine::Shader::Text2D";

		//PostProcessEffect
		const char* const ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_VERTEX_SHADER_PATH = "Learning2DEngine/Shader/DefaultPostProcessEffect.vs";
		const char* const ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_FRAGMENT_SHADER_PATH = "Learning2DEngine/Shader/DefaultPostProcessEffect.fs";
		const std::string ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_NAME = "Learning2DEngine::Shader::DefaultPostProcessEffect";
	}
}