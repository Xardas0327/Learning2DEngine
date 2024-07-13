#pragma once


#include "Learning2DEngine/Render/Shader.h"

class PostProcessData
{
public:
	Learning2DEngine::Render::Shader shader;
	bool confuse;
	bool chaos;
	bool shake;

	PostProcessData(const Learning2DEngine::Render::Shader& shader)
		: shader(shader), confuse(false), chaos(false), shake(false)
	{
        float offset = 1.0f / 300.0f;
        float offsets[9][2] = {
            { -offset,  offset  },  // top-left
            {  0.0f,    offset  },  // top-center
            {  offset,  offset  },  // top-right
            { -offset,  0.0f    },  // center-left
            {  0.0f,    0.0f    },  // center-center
            {  offset,  0.0f    },  // center - right
            { -offset, -offset  },  // bottom-left
            {  0.0f,   -offset  },  // bottom-center
            {  offset, -offset  }   // bottom-right    
        };

        int edge_kernel[9] = {
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1
        };

        float blur_kernel[9] = {
            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
            2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
        };

        this->shader.Use();
        this->shader.SetArray2f("offsets", (float*)offsets, 9);
        this->shader.SetArray1i("edge_kernel", edge_kernel, 9);
        this->shader.SetArray1f("blur_kernel", blur_kernel, 9);
	}

	void RefreshShader(float time)
	{
		shader.Use();
		shader.SetFloat("time", time);
		shader.SetInteger("confuse", confuse);
		shader.SetInteger("chaos", chaos);
		shader.SetInteger("shake", shake);
	}
};