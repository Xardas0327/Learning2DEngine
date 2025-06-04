#include "Text2DRenderData.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Learning2DEngine
{
    namespace UI
    {
        Text2DRenderData::Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, glm::vec4 color)
            : RenderData(component), fontSizePair(fontSizePair), text(""), color(color)
        {
        }

        Text2DRenderData::Text2DRenderData(
            const System::Component* component,
            const FontSizePair& fontSizePair,
            const std::string& text,
            glm::vec4 color
        )
            : RenderData(component), fontSizePair(fontSizePair), text(text), color(color)
        {
        }

        glm::mat2 Text2DRenderData::GetRotationMatrix()
        {
            float radians = glm::radians(component->gameObject->transform.GetRotation());

            return glm::mat2(
                glm::cos(radians), -glm::sin(radians),
                glm::sin(radians), glm::cos(radians)
            );
        }
    }
}