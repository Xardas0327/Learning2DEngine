#include "Text2DRenderData.h"

#include <glm/gtc/matrix_transform.hpp>

#include "TextCharacterSet.h"

namespace Learning2DEngine
{
    namespace UI
    {
        Text2DRenderData::Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, glm::vec4 color)
            : RenderData(component), fontSizePair(fontSizePair), text(""), isModified(true),
            previousModelMatrix(), characterVertices(), color(color), isUseCameraView(false)
        {
        }

        Text2DRenderData::Text2DRenderData(
            const System::Component* component,
            const FontSizePair& fontSizePair,
            const std::string& text,
            glm::vec4 color
        )
            : RenderData(component), fontSizePair(fontSizePair), text(text), isModified(true),
            previousModelMatrix(), characterVertices(), color(color), isUseCameraView(false)
        {
        }

        Text2DRenderData::Text2DRenderData(
            const System::Component* component,
            const FontSizePair& fontSizePair,
            const std::string& text,
            bool isUseCameraView,
            glm::vec4 color
        )
            : RenderData(component), fontSizePair(fontSizePair), text(text), isModified(true),
            previousModelMatrix(), characterVertices(), color(color), isUseCameraView(isUseCameraView)
        {
        }


        glm::mat2 Text2DRenderData::GetRotationMatrix() const
        {
            float radians = glm::radians(component->gameObject->transform.GetRotation());

            return glm::mat2(
                glm::cos(radians), -glm::sin(radians),
                glm::sin(radians), glm::cos(radians)
            );
        }

        void Text2DRenderData::SetText(const std::string& text)
        {
            this->text = text;
            isModified = true;
        }

        void Text2DRenderData::SetText(std::string&& text)
        {
            this->text = std::move(text);
            isModified = true;
        }

        void Text2DRenderData::SetFontSizePair(const FontSizePair& fontSizePair)
        {
            this->fontSizePair = fontSizePair;
            isModified = true;
        }

        std::map<char, std::vector<glm::mat4>> Text2DRenderData::CalculateCharacterVertices() const
        {
            std::map<char, std::vector<glm::mat4>> textMap;
            glm::vec2 startPosition(component->gameObject->transform.GetPosition());
            glm::mat2 rotationMatrix = GetRotationMatrix();

            TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();
            textCharacterSet.Load(fontSizePair);
            CharacterMap& characterMap = textCharacterSet[fontSizePair];

            for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
            {
                const auto& ch = characterMap[*c];

                //Calculcate character position
                float chPositionX = ch.bearing.x * component->gameObject->transform.GetScale().x;
                float chPositionY = (characterMap['H'].bearing.y - ch.bearing.y) * component->gameObject->transform.GetScale().y;

                //Calculcate character size
                float chWidth = ch.size.x * component->gameObject->transform.GetScale().x;
                float chHeight = ch.size.y * component->gameObject->transform.GetScale().y;

                glm::vec2 v1 = startPosition + (rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY + chHeight));
                glm::vec2 v2 = startPosition + (rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY));
                glm::vec2 v3 = startPosition + (rotationMatrix * glm::vec2(chPositionX, chPositionY));
                glm::vec2 v4 = startPosition + (rotationMatrix * glm::vec2(chPositionX, chPositionY + chHeight));

                textMap[*c].push_back(glm::mat4(
                    v1.x, v1.y, 1.0f, 1.0f,
                    v2.x, v2.y, 1.0f, 0.0f,
                    v3.x, v3.y, 0.0f, 0.0f,
                    v4.x, v4.y, 0.0f, 1.0f
                ));

                // Calculate next character position
                startPosition += rotationMatrix *
                    glm::vec2(
                        // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
                        (ch.advance >> 6) * component->gameObject->transform.GetScale().x,
                        0);
            }

            return textMap;
        }

        const std::map<char, std::vector<glm::mat4>>& Text2DRenderData::GetCharacterVertices()
        {
            auto& actualModelMatrix = component->gameObject->transform.GetModelMatrix();
            if (isModified || previousModelMatrix != actualModelMatrix)
            {
                characterVertices = CalculateCharacterVertices();
                previousModelMatrix = actualModelMatrix;
                isModified = false;
            }

            return characterVertices;
        }

        std::map<char, std::vector<glm::mat4>> Text2DRenderData::GetCharacterVertices() const
        {
            auto& actualModelMatrix = component->gameObject->transform.GetModelMatrix();
            if (isModified || previousModelMatrix != actualModelMatrix)
                return CalculateCharacterVertices();

            return characterVertices;
        }
        
        glm::vec2 Text2DRenderData::GetTextLength() const
        {
            TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();
            textCharacterSet.Load(fontSizePair);
            CharacterMap& characterMap = textCharacterSet[fontSizePair];

			glm::vec2 length(0.0f, characterMap['H'].bearing.y * component->gameObject->transform.GetScale().y);
            for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
            {
                const auto& ch = characterMap[*c];
                length.x += (ch.advance >> 6) * component->gameObject->transform.GetScale().x;
            }

            return length;
        }
    }
}