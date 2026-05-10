#include "Transform.h"

namespace Learning2DEngine
{
	namespace System
	{
		Transform::Transform(glm::vec2 position, glm::vec2 scale, float rotation)
			: position(position), scale(scale), rotation(rotation), isModified(true), modelMatrix()
		{

		}

		glm::mat4 Transform::CalculateModelMatrix() const
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

		void Transform::SetPosition(const glm::vec2& newPosition)
		{
			position = newPosition;
			isModified = true;
		}

		void Transform::AddPosition(const glm::vec2& deltaPosition)
		{
			position += deltaPosition;
			isModified = true;
		}

		void Transform::SetScale(const glm::vec2& newScale)
		{
			scale = newScale;
			isModified = true;
		}

		void Transform::AddScale(const glm::vec2& deltaScale)
		{
			scale += deltaScale;
			isModified = true;
		}

		void Transform::SetRotation(float newRotation)
		{
			rotation = newRotation;
			isModified = true;
		}

		void Transform::AddRotation(float deltaRotation)
		{
			rotation += deltaRotation;
			isModified = true;
		}

		const glm::mat4& Transform::GetModelMatrix()
		{
			if (isModified)
			{
				modelMatrix = CalculateModelMatrix();
				isModified = false;
			}

			return modelMatrix;
		}

		glm::mat4 Transform::GetModelMatrix() const
		{
			if (isModified)
				return CalculateModelMatrix();

			return modelMatrix;
		}
	}
}