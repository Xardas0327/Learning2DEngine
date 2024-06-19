#pragma once

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../System/Singleton.h"
#include "Shader.h"
#include "Resolution.h"
#include "../UI/FreeTypeCharacter.h"
#include "../UI/Text.h"

namespace Learning2DEngine
{
    namespace Render
    {
#define FONT_NUMBER 128

        class Text2DRenderer : public virtual System::Singleton<Text2DRenderer>
        {
            friend class System::Singleton<Text2DRenderer>;
            typedef std::map<char, UI::FreeTypeCharacter> CharacterMap;
        private:
            unsigned int vao;
            unsigned int vbo;
            unsigned int ebo;
            std::map<UI::FontSizePair, CharacterMap> characters;
            Shader textShader;

            Text2DRenderer();

            // Callbacks

            static void CallbackRefreshScreenResolution(Resolution resolution);
            void RefreshScreenResolution(Resolution resolution);
        public:
            /// <summary>
            /// It should be inited after RenderManager
            /// </summary>
            void Init();
            /// <summary>
            /// It loads FONT_NUMBER characters from the file begining
            /// </summary>
            void Load(const std::string& font, unsigned int fontSize);
            /// <summary>
            /// It loads FONT_NUMBER characters from the file begining
            /// </summary>
            void Load(const UI::FontSizePair& fontSizePair);
            void Unload(const std::string& font, unsigned int fontSize);
            void Unload(const UI::FontSizePair& fontSizePair);
            void Clear();
            void RenderText(const UI::Text& text);
        };
    }
}