#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace Learning2DEngine
{
	namespace System
	{
		class GameObject;

		class Transform
		{
		private:
			glm::vec2 position;
			glm::vec2 scale;
			float rotation;

			mutable glm::vec2 globalPosition;
			mutable glm::vec2 globalScale;
			mutable float globalRotation;
			mutable bool isModified;
			mutable glm::mat4 modelMatrix;

			GameObject* gameObject;
			GameObject* parent;
			std::vector<GameObject*> children;

			glm::mat4 CalculateLocalModelMatrix() const;
			void UpdateCachedData() const;
			bool IsModified() const;
		public:
			//deprecated
			Transform(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f);

			Transform(
				GameObject* gameObject,
				glm::vec2 position = glm::vec2(0.0f, 0.0f),
				glm::vec2 scale = glm::vec2(1.0f, 1.0f),
				float rotation = 0.0f
			);

			inline glm::vec2 GetLocalPosition() const
			{
				return position;
			}

			void SetLocalPosition(const glm::vec2& newPosition);

			void AddLocalPosition(const glm::vec2& deltaPosition);

			glm::vec2 GetGlobalPosition() const;

			inline glm::vec2 GetLocalScale() const
			{
				return scale;
			}

			void SetLocalScale(const glm::vec2& newScale);

			void AddLocalScale(const glm::vec2& deltaScale);

			glm::vec2 GetGlobalScale() const;

			inline float GetLocalRotation() const
			{
				return rotation;
			}

			void SetLocalRotation(float newRotation);

			void AddLocalRotation(float deltaRotation);

			float GetGlobalRotation() const;

			const glm::mat4& GetModelMatrix() const;

			inline GameObject* GetParent() const
			{
				return parent;
			}

			void SetParent(GameObject* newParent);

			void ClearParent();
		};
	}
}

