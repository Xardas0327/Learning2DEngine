#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Render/RenderData.h"
#include "FontSizePair.h"

namespace Learning2DEngine
{
    namespace UI
    {
        struct Text2DRenderData : public Render::RenderData
        {
            FontSizePair fontSizePair;
            std::string text;
            glm::vec4 color;

            Text2DRenderData(const System::Component* component, const glm::vec4 color = glm::vec4(1.0f))
                : RenderData(component), fontSizePair(), text(""), color(color)
            {
            }

            Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, const std::string& text, glm::vec4 color = glm::vec4(1.0f))
                : RenderData(component), fontSizePair(fontSizePair), text(text), color(color)
            {
            }

            glm::mat2 GetRotationMatrix()
            {
                float radians = glm::radians(component->gameObject->transform.rotation);

                return glm::mat2(
                    glm::cos(radians), -glm::sin(radians),
                    glm::sin(radians), glm::cos(radians)
                );
            }
        };
    }
}