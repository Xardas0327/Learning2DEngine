#pragma once

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../System/Singleton.h"
#include "../Render/Shader.h"
#include "../Render/Resolution.h"
#include "FreeTypeCharacter.h"
#include "Text.h"

namespace Learning2DEngine
{
    namespace UI
    {
#define FONT_NUMBER 128

        class Text2DRenderer : public virtual System::Singleton<Text2DRenderer>
        {
            friend class System::Singleton<Text2DRenderer>;
            typedef std::map<char, FreeTypeCharacter> CharacterMap;
        private:
            unsigned int vao;
            unsigned int vbo;
            unsigned int ebo;
            std::map<FontSizePair, CharacterMap> characters;
            Render::Shader textShader;

            Text2DRenderer();
        public:
            /// <summary>
            /// It should be inited after RenderManager.
            /// It will enable the blend, where the source factor is GL_SRC_ALPHA 
            /// and destination factor is GL_ONE_MINUS_SRC_ALPHA
            /// </summary>
            void Init(const Render::Resolution& resolution);
            /// <summary>
            /// It should be terminated before RenderManager
            /// </summary>
            void Terminate();
            /// <summary>
            /// It loads FONT_NUMBER characters from the file begining
            /// </summary>
            void Load(const std::string& font, unsigned int fontSize);
            /// <summary>
            /// It loads FONT_NUMBER characters from the file begining
            /// </summary>
            void Load(const FontSizePair& fontSizePair);
            void Unload(const std::string& font, unsigned int fontSize);
            void Unload(const FontSizePair& fontSizePair);
            void Clear();
            void RenderText(const Text& text);
        };
    }
}