#pragma once

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../System/Singleton.h"
#include "../Render/Shader.h"
#include "FreeTypeCharacter.h"

namespace Learning2DEngine
{
    namespace UI
    {
        class Text2DRenderer : public virtual System::Singleton<Text2DRenderer>
        {
            friend class System::Singleton<Text2DRenderer>;
        public:
            typedef std::pair<std::string, unsigned int> FontSizePair;
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
            /// <param name="font"></param>
            /// <param name="fontSize"></param>
            void Load(std::string font, unsigned int fontSize);
            void Unload(std::string font, unsigned int fontSize);
            void Clear();
            void RenderText(std::string font, unsigned int fontSize, std::string text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));
        };
    }
}