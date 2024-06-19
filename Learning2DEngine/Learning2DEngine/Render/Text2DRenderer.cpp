#include "Text2DRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../System/ResourceManager.h"
#include "../System/Log.h"
#include "RenderManager.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;

namespace Learning2DEngine
{
    namespace Render
    {
        Text2DRenderer::Text2DRenderer() :
            vao(0), vbo(0), ebo(0), characters(), textShader()
        {

        }

        void Text2DRenderer::Init()
        {
            auto& renderManager = RenderManager::GetInstance();

            textShader = ResourceManager::LoadShader("Learning2DEngine/Shaders/Text2D.vs", "Learning2DEngine/Shaders/Text2D.fs");
            textShader.Use();
            textShader.SetMatrix4(
                "projection",
                glm::ortho(
                    0.0f,
                    static_cast<float>(renderManager.GetResolution().GetWidth()),
                    static_cast<float>(renderManager.GetResolution().GetHeight()),
                    0.0f));
            textShader.SetInteger("text", 0);

            unsigned int indices[] = {
                0, 1, 3,
                1, 2, 3
            };

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            renderManager.AddFramebufferSizeEvent(CallbackRefreshScreenResolution);
        }

        void Text2DRenderer::CallbackRefreshScreenResolution(Resolution resolution)
        {
            Text2DRenderer::GetInstance().RefreshScreenResolution(resolution);
        }

        void Text2DRenderer::RefreshScreenResolution(Resolution resolution)
        {
            textShader.SetMatrix4(
                "projection",
                glm::ortho(
                    0.0f,
                    static_cast<float>(resolution.GetWidth()),
                    static_cast<float>(resolution.GetHeight()),
                    0.0f));
        }

        void Text2DRenderer::Load(const std::string& font, unsigned int fontSize)
        {
            Load(FontSizePair(font, fontSize));
        }

        void Text2DRenderer::Load(const FontSizePair& fontSizePair)
        {
            // fontSizePair.first is the font name
            // fontSizePair.second is the size
            if (characters.count(fontSizePair))
                return;

            FT_Library ft;
            if (FT_Init_FreeType(&ft))
            {
                LOG_ERROR("FREETYPE: Could not init FreeType Library");
                return;
            }

            FT_Face face;

            if (FT_New_Face(ft, fontSizePair.first.c_str(), 0, &face))
            {
                LOG_ERROR("FREETYPE: Failed to load font: " + fontSizePair.first);
                return;
            }

            FT_Set_Pixel_Sizes(face, 0, fontSizePair.second);
            characters.insert(std::pair<FontSizePair, CharacterMap>(fontSizePair, CharacterMap()));

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            for (GLubyte ch = 0; ch < FONT_NUMBER; ++ch)
            {
                if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
                {
                    LOG_WARNING("FREETYTPE: Failed to load Glyph: " + ch);
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

        void Text2DRenderer::Unload(const std::string& font, unsigned int fontSize)
        {
            Unload(FontSizePair(font, fontSize));
        }

        void Text2DRenderer::Unload(const FontSizePair& fontSizePair)
        {
            if (!characters.count(fontSizePair))
                return;

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

        void Text2DRenderer::RenderText(const Text& text)
        {
            Load(text.fontSizePair);

            CharacterMap& characterMap = characters[text.fontSizePair];

            textShader.Use();
            textShader.SetVector3f("textColor", text.color);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao);

            std::string::const_iterator c;
            int xPosition = text.x;
            for (c = text.text.begin(); c != text.text.end(); c++)
            {
                auto& ch = characterMap[*c];

                float xpos = xPosition + ch.bearing.x * text.scale;
                float ypos = text.y + (characterMap['H'].bearing.y - ch.bearing.y) * text.scale;

                float w = ch.size.x * text.scale;
                float h = ch.size.y * text.scale;

                float vertices[4][4] = {
                    { xpos + w, ypos + h,   1.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 0.0f },
                    { xpos,     ypos + h,   0.0f, 1.0f }
                };

                glBindTexture(GL_TEXTURE_2D, ch.textureId);

                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
                xPosition += (ch.advance >> 6) * text.scale;
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}