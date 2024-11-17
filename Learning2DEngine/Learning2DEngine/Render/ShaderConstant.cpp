#include "ShaderConstant.h"

namespace Learning2DEngine
{
	namespace Render
	{
		//Sprite
		const std::string ShaderConstant::SPRITE_SHADER_NAME = "Learning2DEngine::Shader::Sprite";
		const char* const ShaderConstant::SPRITE_VERTEX_SHADER =
			R"(#version 330 core
			layout(location = 0) in vec2 postion;
			layout(location = 1) in vec2 textureCoords;

			out vec2 TextureCoords;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * view * model * vec4(postion, 0.0, 1.0);
				TextureCoords = textureCoords;
			})";
		const char* const ShaderConstant::SPRITE_FRAGMENT_SHADER =
			R"(#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform bool isUseTexture;
			uniform sampler2D spriteTexture;
			uniform vec3 spriteColor;

			void main()
			{
				color = vec4(spriteColor, 1.0);
				if (isUseTexture)
				{
					color *= texture(spriteTexture, TextureCoords);
				}
			})";

		//Text2D
		const std::string ShaderConstant::TEXT2D_SHADER_NAME = "Learning2DEngine::Shader::Text2D";
		const char* const ShaderConstant::TEXT2D_VERTEX_SHADER =
			R"(#version 330 core
			layout(location = 0) in vec2 postion;
			layout(location = 1) in vec2 textureCoords;

			out vec2 TextureCoords;

			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * vec4(postion, 0.0, 1.0);
				TextureCoords = textureCoords;
			})";
		const char* const ShaderConstant::TEXT2D_FRAGMENT_SHADER =
			R"(#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform sampler2D characterTexture;
			uniform vec3 characterColor;

			void main()
			{
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(characterTexture, TextureCoords).r);
				color = vec4(characterColor, 1.0) * sampled;
			})";

		//PostProcessEffect
		const std::string ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_NAME = "Learning2DEngine::Shader::DefaultPostProcessEffect";
		const char* const ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_VERTEX_SHADER =
			R"(#version 330 core
			layout(location = 0) in vec2 postion;
			layout(location = 1) in vec2 textureCoords;

			out vec2 TextureCoords;

			void main()
			{
				gl_Position = vec4(postion, 0.0, 1.0);
				TextureCoords = textureCoords;
			})";
		const char* const ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_FRAGMENT_SHADER =
			R"(#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform sampler2D scene;

			void main()
			{
				color = texture(scene, TextureCoords);
			})";
	}
}