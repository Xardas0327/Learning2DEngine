#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../Render/RenderData.h"
#include "FontSizePair.h"

namespace Learning2DEngine
{
    namespace UI
    {
        class Text2DRenderData : public Render::RenderData
        {
        protected:
            std::string text;

        public:
            FontSizePair fontSizePair;
            glm::vec4 color;

            Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, glm::vec4 color = glm::vec4(1.0f));

            Text2DRenderData(
                const System::Component* component,
                const FontSizePair& fontSizePair,
                const std::string& text,
                glm::vec4 color = glm::vec4(1.0f));

            glm::mat2 GetRotationMatrix();

            inline const std::string& GetText() const
            {
                return text;
            }

            inline void SetText(const std::string& text)
            {
                this->text = text;
            }

            inline void SetText(std::string&& text)
            {
                this->text = std::move(text);
            }
        };
    }
}