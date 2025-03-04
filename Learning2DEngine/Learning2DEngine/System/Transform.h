#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Learning2DEngine
{
	namespace System
	{
		struct Transform
		{
			glm::vec2 position;
			glm::vec2 scale;
			float rotation;

			Transform(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f)
				: position(position), scale(scale), rotation(rotation)
			{

			}

			glm::mat4 GetModelMatrix()
			{
				glm::mat4 model = glm::mat4(1.0f);
				// first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
				model = glm::translate(model, glm::vec3(position, 0.0f));
				// move origin of rotation to center of quad
				model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
				// then rotate
				model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
				// move origin back
				model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
				// then rotate
				model = glm::scale(model, glm::vec3(scale, 1.0f)); // last scale

				return model;
			}
		};
	}
}

