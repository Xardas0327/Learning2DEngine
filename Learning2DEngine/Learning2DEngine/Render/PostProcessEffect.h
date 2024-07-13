#pragma once

#include "Resolution.h"
#include "Texture2D.h"
#include "Shader.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class PostProcessEffect
		{
		private:
			Shader defaultShader;
			Shader effectShader;
			Texture2D texture;
			unsigned int vao;
			unsigned int vbo;
			unsigned int ebo;
			unsigned int fbo;
			bool isEffectUsed;

			void InitVao();
			void InitDefaultShader();
		public:
			PostProcessEffect();
			~PostProcessEffect();

			void Init(Resolution resolution);
			void Destroy();

			void StartRender(bool useClear = true);
			void EndRender();

			void Render();

			void SetShader(const Shader& shader);
			void ClearShader();

			inline unsigned int GetFrameBufferId()
			{
				return fbo;
			}
		};
	}
}