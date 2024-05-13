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
            textShader = ResourceManager::GetInstance().LoadShader("Shaders/text_2d.vs", "Shaders/text_2d.fs", nullptr, "Text2DRenderer");
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
            characters.clear();

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
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            for (GLubyte ch = 0; ch < 128; ch++)
            {
                if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
                {
                    throw std::exception("ERROR::FREETYTPE: Failed to load Glyph");
                    //This one should be only warning
                    //continue;
                }

                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
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
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };
                characters.insert(std::pair<char, FreeTypeCharacter>(ch, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }

        void Text2DRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
        {
            textShader.Use();
            textShader.SetVector3f("textColor", color);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao);

            std::string::const_iterator c;
            for (c = text.begin(); c != text.end(); c++)
            {
                auto& ch = characters[*c];

                float xpos = x + ch.bearing.x * scale;
                float ypos = y + (characters['H'].bearing.y - ch.bearing.y) * scale;

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