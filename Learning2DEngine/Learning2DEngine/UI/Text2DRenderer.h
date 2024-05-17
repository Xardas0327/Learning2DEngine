#pragma once

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../System/Singleton.h"
#include "../Render/Shader.h"
#include "FreeTypeCharacter.h"
#include "Text.h"

namespace Learning2DEngine
{
    namespace UI
    {
        class Text2DRenderer : public virtual System::Singleton<Text2DRenderer>
        {
            friend class System::Singleton<Text2DRenderer>;
            typedef std::map<char, FreeTypeCharacter> CharacterMap;
        private:
            unsigned int vao, vbo, ebo;
            std::map<FontSizePair, CharacterMap> characters;
            Render::Shader textShader;

            Text2DRenderer();

            // Callbacks

            static void CallbackRefreshScreenSize(int width, int height);
            void RefreshScreenSize(int width, int height);
        public:
            /// <summary>
            /// It should be inited after RenderManager
            /// </summary>
            void Init();
            /// <summary>
            /// It loads the first 128 characters from the file
            /// </summary>
            void Load(const std::string& font, unsigned int fontSize);
            /// <summary>
            /// It loads the first 128 characters from the file
            /// </summary>
            void Load(const FontSizePair& fontSizePair);
            void Unload(const std::string& font, unsigned int fontSize);
            void Unload(const FontSizePair& fontSizePair);
            void Clear();
            void RenderText(const Text& text);
        };
    }
}