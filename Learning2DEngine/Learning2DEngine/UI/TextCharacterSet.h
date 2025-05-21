#pragma once

#include <map>

#include "../System/Singleton.h"
#include "FreeTypeCharacter.h"
#include "FontSizePair.h"

namespace Learning2DEngine
{
    namespace UI
    {
#define L2DE_FONT_NUMBER 128

        typedef std::map<char, FreeTypeCharacter> CharacterMap;

        class TextCharacterSet : public System::Singleton<TextCharacterSet>
        {
            friend class System::Singleton<TextCharacterSet>;
        private:
            std::map<FontSizePair, CharacterMap> characters;

            TextCharacterSet();
        public:
            CharacterMap& operator[](const FontSizePair& fontSizePair);
            const CharacterMap& operator[](const FontSizePair& fontSizePair) const;
            /// <summary>
            /// It loads L2DE_FONT_NUMBER characters from the file begining
            /// </summary>
            void Load(const std::string& font, unsigned int fontSize);
            /// <summary>
            /// It loads L2DE_FONT_NUMBER characters from the file begining
            /// </summary>
            void Load(const FontSizePair& fontSizePair);
            void Unload(const std::string& font, unsigned int fontSize);
            void Unload(const FontSizePair& fontSizePair);
            void Clear();
        };
    }
}