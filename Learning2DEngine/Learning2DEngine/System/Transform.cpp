#include "Transform.h"

#include "GameObject.h"

namespace Learning2DEngine
{
	namespace System
	{
		Transform::Transform(glm::vec2 position, glm::vec2 scale, float rotation)
			: position(position), scale(scale), rotation(rotation),
			globalPosition(), globalScale(), globalRotation(),
			isModified(true), modelMatrix(), gameObject(nullptr), parent(nullptr), children()
		{

		}

		Transform::Transform(GameObject* gameObject, glm::vec2 position, glm::vec2 scale, float rotation)
			: position(position), scale(scale), rotation(rotation),
			globalPosition(), globalScale(), globalRotation(),
			isModified(true), modelMatrix(), gameObject(gameObject), parent(nullptr), children()
		{

		}

		glm::mat4 Transform::CalculateLocalModelMatrix() const
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

		void Transform::UpdateCachedData() const
		{
			if (isModified)
			{
				if (parent == nullptr)
				{
					modelMatrix = CalculateLocalModelMatrix();

					globalPosition = position;
					globalScale = scale;
					globalRotation = rotation;
				}
				else
				{
					modelMatrix = parent->transform.GetModelMatrix() * CalculateLocalModelMatrix();

					globalPosition = glm::vec2(modelMatrix[3][0], modelMatrix[3][1]);

					float scaleX = glm::length(glm::vec2(modelMatrix[0][0], modelMatrix[0][1]));
					float scaleY = glm::length(glm::vec2(modelMatrix[1][0], modelMatrix[1][1]));
					globalScale = glm::vec2(scaleX, scaleY);

					globalRotation = atan2(modelMatrix[1][0] / scaleY, modelMatrix[0][0] / scaleX);
				}

				isModified = false;
			}
		}

		void Transform::MarkAsModified()
		{
			isModified = true;
			for (GameObject* child : children)
			{
				child->transform.MarkAsModified();
			}
		}

		void Transform::SetLocalPosition(const glm::vec2& newPosition)
		{
			position = newPosition;
			MarkAsModified();
		}

		void Transform::AddLocalPosition(const glm::vec2& deltaPosition)
		{
			position += deltaPosition;
			MarkAsModified();
		}

		glm::vec2 Transform::GetGlobalPosition() const
		{
			UpdateCachedData();

			return globalPosition;
		}

		void Transform::SetLocalScale(const glm::vec2& newScale)
		{
			scale = newScale;
			MarkAsModified();
		}

		void Transform::AddLocalScale(const glm::vec2& deltaScale)
		{
			scale += deltaScale;
			MarkAsModified();
		}

		glm::vec2 Transform::GetGlobalScale() const
		{
			UpdateCachedData();

			return globalScale;
		}

		void Transform::SetLocalRotation(float newRotation)
		{
			rotation = newRotation;
			MarkAsModified();
		}

		void Transform::AddLocalRotation(float deltaRotation)
		{
			rotation += deltaRotation;
			MarkAsModified();
		}

		float Transform::GetGlobalRotation() const
		{
			UpdateCachedData();

			return globalRotation;
		}

		const glm::mat4& Transform::GetModelMatrix() const
		{
			UpdateCachedData();

			return modelMatrix;
		}

		void Transform::SetParent(GameObject* newParent)
		{
			ClearParent();

			parent = newParent;
			if (parent != nullptr)
			{
				parent->transform.children.push_back(gameObject);
				MarkAsModified();
			}
		}

		void Transform::ClearParent()
		{
			if (parent != nullptr)
			{
				auto& siblings = parent->transform.children;
				siblings.erase(std::remove(siblings.begin(), siblings.end(), gameObject), siblings.end());

				parent = nullptr;
				MarkAsModified();
			}
		}
	}
}