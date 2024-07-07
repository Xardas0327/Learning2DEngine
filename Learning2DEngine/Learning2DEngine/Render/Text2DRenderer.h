#pragma once

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../System/Singleton.h"
#include "../System/FreeTypeCharacter.h"
#include "../System/Text.h"
#include "Shader.h"
#include "Resolution.h"

namespace Learning2DEngine
{
    namespace Render
    {
#define FONT_NUMBER 128

        class Text2DRenderer : public virtual System::Singleton<Text2DRenderer>
        {
            friend class System::Singleton<Text2DRenderer>;
            typedef std::map<char, System::FreeTypeCharacter> CharacterMap;
        private:
            unsigned int vao;
            unsigned int vbo;
            unsigned int ebo;
            std::map<System::FontSizePair, CharacterMap> characters;
            Shader textShader;

            Text2DRenderer();

            // Callbacks

            static void CallbackRefreshWindowResolution(Resolution resolution);
            void RefreshWindowResolution(Resolution resolution);
        public:
            /// <summary>
            /// It should be inited after RenderManager.
            /// It will enable the blend, where the source factor is GL_SRC_ALPHA 
            /// and destination factor is GL_ONE_MINUS_SRC_ALPHA
            /// </summary>
            void Init();
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
            void Load(const System::FontSizePair& fontSizePair);
            void Unload(const std::string& font, unsigned int fontSize);
            void Unload(const System::FontSizePair& fontSizePair);
            void Clear();
            void RenderText(const System::Text& text);
        };
    }
}