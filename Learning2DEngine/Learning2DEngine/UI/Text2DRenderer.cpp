#include "Text2DRenderer.h"

#include <exception>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../System/ResourceManager.h"

using namespace Learning2DEngine::System;

namespace Learning2DEngine
{
    namespace UI
    {
        Text2DRenderer::Text2DRenderer(unsigned int width, unsigned int height)
        {
            textShader = ResourceManager::LoadShader("Shaders/text_2d.vs", "Shaders/text_2d.fs");
            textShader.Use();
            textShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f));
            textShader.SetInteger("text", 0);

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void Text2DRenderer::Load(std::string font, unsigned int fontSize)
        {
            const FontSizePair  fontSizePair = FontSizePair(font, fontSize);
            if (characters.count(fontSizePair))
                return;

            FT_Library ft;
            if (FT_Init_FreeType(&ft))
            {
                throw std::exception("ERROR::FREETYPE: Could not init FreeType Library");
            }

            FT_Face face;
            if (FT_New_Face(ft, font.c_str(), 0, &face))
            {
                throw std::exception("ERROR::FREETYPE: Failed to load font");
            }

            FT_Set_Pixel_Sizes(face, 0, fontSize);
            characters.insert(std::pair<FontSizePair, CharacterMap>(fontSizePair, CharacterMap()));

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            for (GLubyte ch = 0; ch < 128; ch++)
            {
                if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
                {
                    //This one should be only warning
                    //throw std::exception("ERROR::FREETYTPE: Failed to load Glyph");
                    continue;
                }

                unsigned int textureId;
                glGenTextures(1, &textureId);
                glBindTexture(GL_TEXTURE_2D, textureId);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                FreeTypeCharacter character = {
                    textureId,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };
                characters[fontSizePair].insert(std::pair<char, FreeTypeCharacter>(ch, character));
            }
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glBindTexture(GL_TEXTURE_2D, 0);

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }

        void Text2DRenderer::Unload(std::string font, unsigned int fontSize)
        {
            const FontSizePair fontSizePair = FontSizePair(font, fontSize);
            if (characters.count(fontSizePair))
            {
                const CharacterMap& characterMap = characters[fontSizePair];
                unsigned int* textureIds = new unsigned int[characterMap.size()];
                unsigned int index = 0;
                for (auto i = characterMap.begin(); i != characterMap.end(); ++i)
                {
                    textureIds[index] = i->second.textureId;
                    ++index;
                }

                glDeleteTextures(index, textureIds);
                delete[] textureIds;

                characters.erase(fontSizePair);
            }
        }

        void Text2DRenderer::Clear()
        {
            for (auto i = characters.begin(); i != characters.end(); ++i)
            {
                unsigned int* textureIds = new unsigned int[i->second.size()];
                unsigned int index = 0;
                for (auto characterPair = i->second.begin(); characterPair != i->second.end(); ++characterPair)
                {
                    textureIds[index] = characterPair->second.textureId;
                    ++index;
                }

                glDeleteTextures(index, textureIds);
                delete[] textureIds;
            }
            characters.clear();
        }

        void Text2DRenderer::RenderText(std::string font, unsigned int fontSize, std::string text, float x, float y, float scale, glm::vec3 color)
        {
            Load(font, fontSize);
            const FontSizePair fontSizePair = FontSizePair(font, fontSize);
            CharacterMap& characterMap = characters[fontSizePair];

            textShader.Use();
            textShader.SetVector3f("textColor", color);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao);

            std::string::const_iterator c;
            for (c = text.begin(); c != text.end(); c++)
            {
                auto& ch = characterMap[*c];

                float xpos = x + ch.bearing.x * scale;
                float ypos = y + (characterMap['H'].bearing.y - ch.bearing.y) * scale;

                float w = ch.size.x * scale;
                float h = ch.size.y * scale;

                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 0.0f },

                    { xpos,     ypos + h,   0.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 0.0f }
                };

                glBindTexture(GL_TEXTURE_2D, ch.textureId);

                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glDrawArrays(GL_TRIANGLES, 0, 6);

                // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
                x += (ch.advance >> 6) * scale; 
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}