#pragma once

#include "Resolution.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class MSAA
		{
		private:
			unsigned int fbo;
			unsigned int rbo;
			unsigned int sampleNumber;
			Resolution resolution;
		public:
			MSAA();
			~MSAA();
			void Init(unsigned int sampleNumber, Resolution resolution);
			void Destroy();
			void StartRender();
			/// <summary>
			/// The MSAA color-buffer will be moved into resultFbo framebuffer.
			/// </summary>
			void EndRender(unsigned int resultFbo, Resolution  resultResolution);

			inline unsigned int GetId()
			{
				return fbo;
			}

			inline unsigned int GetSampleNumber()
			{
				return sampleNumber;
			}
		};
	}
}