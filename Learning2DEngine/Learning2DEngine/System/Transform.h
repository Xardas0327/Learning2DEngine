#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Learning2DEngine
{
	namespace System
	{
		class Transform
		{
		private:
			glm::vec2 position;
			glm::vec2 scale;
			float rotation;
			bool isModified;
			glm::mat4 modelMatrix;

			glm::mat4 CalculateModelMatrix() const;
		public:
			Transform(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f);

			inline glm::vec2 GetPosition() const
			{
				return position;
			}

			void SetPosition(const glm::vec2& newPosition);

			void AddPosition(const glm::vec2& deltaPosition);

			inline glm::vec2 GetScale() const
			{
				return scale;
			}

			void SetScale(const glm::vec2& newScale);

			void AddScale(const glm::vec2& deltaScale);

			inline float GetRotation() const
			{
				return rotation;
			}

			void SetRotation(float newRotation);

			void AddRotation(float deltaRotation);

			const glm::mat4& GetModelMatrix();

			glm::mat4 GetModelMatrix() const;
		};
	}
}

