#include "ShaderConstant.h"

#include <string>

#include "RenderManager.h"

namespace Learning2DEngine
{
	namespace Render
	{
		//Simple Sprite

		const std::string ShaderConstant::SIMPLE_SPRITE_SHADER_NAME = "L2DE_SimpleSprite";

		const char* ShaderConstant::GetSimpleSpriteVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
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

			return shader;
		}

		const char* ShaderConstant::GetSimpleSpriteFragmentShader()
		{
			static const char* shader = R"(
			#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform bool isUseTexture;
			uniform sampler2D spriteTexture;
			uniform vec4 spriteColor;

			void main()
			{
				color = spriteColor;
				if (isUseTexture)
				{
					color *= texture(spriteTexture, TextureCoords);
				}
			})";

			return shader;
		}

		//Old Sprite (Multi instancing support)
		const std::string ShaderConstant::OLD_SPRITE_SHADER_NAME = "L2DE_OldSprite";

		const char* ShaderConstant::GetOldSpriteVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 postion;
			layout(location = 1) in vec2 textureCoords;
			layout(location = 2) in mat4 model;
			layout(location = 6) in vec4 spriteColor;

			out vec2 TextureCoords;
			out vec4 SpriteColor;

			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * view * model * vec4(postion, 0.0, 1.0);
				TextureCoords = textureCoords;
				SpriteColor = spriteColor;
			})";

			return shader;
		}

		const char* ShaderConstant::GetOldSpriteFragmentShader()
		{
			static const char* shader = R"(
			#version 330 core
			in vec2 TextureCoords;
			in vec4 SpriteColor;

			out vec4 color;

			uniform bool isUseTexture;
			uniform sampler2D spriteTexture;

			void main()
			{
				color = SpriteColor;
				if (isUseTexture)
				{
					color *= texture(spriteTexture, TextureCoords);
				}
			})";

			return shader;
		}

		//Sprite (Multi instancing support)

		const std::string ShaderConstant::SPRITE_SHADER_NAME = "L2DE_Sprite";

		const char* ShaderConstant::GetSpriteVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 postion;
			layout(location = 1) in vec2 textureCoords;
			layout(location = 2) in mat4 model;
			layout(location = 6) in vec4 spriteColor;
			layout(location = 7) in int spriteTextureId;

			out vec2 TextureCoords;
			out vec4 SpriteColor;
			flat out int SpriteTextureId;

			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * view * model * vec4(postion, 0.0, 1.0);
				TextureCoords = textureCoords;
				SpriteColor = spriteColor;
				SpriteTextureId = spriteTextureId;
			})";

			return shader;
		}

		const char* ShaderConstant::GetSpriteFragmentShader()
		{
			GLint maxUnitNumber = RenderManager::GetInstance().GetMaxTextureUnits();

			static std::string shader = R"(
			#version 330 core
			in vec2 TextureCoords;
			in vec4 SpriteColor;
			flat in int SpriteTextureId;

			out vec4 color;

			uniform sampler2D[)" + std::to_string(maxUnitNumber) + R"(] spriteTextures;

			void main()
			{
				color = SpriteColor;
				if (SpriteTextureId>=0 && SpriteTextureId <)" + std::to_string(maxUnitNumber) + R"() 
				{
					color *= texture(spriteTextures[SpriteTextureId], TextureCoords);
				}
			})";

			return shader.c_str();
		}

		//Text2D

		const std::string ShaderConstant::TEXT2D_SHADER_NAME = "L2DE_Text2D";

		const char* ShaderConstant::GetText2DVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 postion;
			layout(location = 1) in vec2 textureCoords;

			out vec2 TextureCoords;

			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * vec4(postion, 0.0, 1.0);
				TextureCoords = textureCoords;
			})";

			return shader;
		}

		const char* ShaderConstant::GetText2DFragmentShader()
		{
			static const char* shader = R"(
			#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform sampler2D characterTexture;
			uniform vec4 characterColor;

			void main()
			{
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(characterTexture, TextureCoords).r);
				color = characterColor * sampled;
			})";

			return shader;
		}

		//PostProcessEffect

		const std::string ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_NAME = "L2DE_DefaultPostProcessEffect";

		const char* ShaderConstant::GetDefaultPostprocessVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 postion;
			layout(location = 1) in vec2 textureCoords;

			out vec2 TextureCoords;

			void main()
			{
				gl_Position = vec4(postion, 0.0, 1.0);
				TextureCoords = textureCoords;
			})";

			return shader;
		}

		const char* ShaderConstant::GetDefaultPostprocessFragmentShader()
		{
			static const char* shader = R"(
			#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform sampler2D scene;

			void main()
			{
				color = texture(scene, TextureCoords);
			})";

			return shader;
		}
	}
}