#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Learning2DEngine/Render/Shader.h"
#include "Learning2DEngine/Render/Texture2D.h"


// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or 
// Shake boolean. 
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
class PostProcessor
{
public:
    // state
    Learning2DEngine::Render::Shader PostProcessingShader;
    Learning2DEngine::Render::Texture2D Texture;
    unsigned int Width, Height;
    // options
    bool Confuse, Chaos, Shake;
    // constructor
    PostProcessor(Learning2DEngine::Render::Shader shader, unsigned int width, unsigned int height);
    // renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void Render(float time);

    inline unsigned int GetFbo()
    {
        return FBO;
    }
private:
    // render state
    unsigned int FBO;
    unsigned int VAO;
    // initialize quad for rendering postprocessing texture
    void initRenderData();
};