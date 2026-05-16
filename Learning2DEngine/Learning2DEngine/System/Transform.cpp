#include "Transform.h"

#include "GameObject.h"
#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
	namespace System
	{
		Transform::Transform(GameObject* gameObject, glm::vec2 position, glm::vec2 scale, float rotation)
			: localPosition(position), localScale(scale), localRotation(rotation),
			globalPosition(), globalScale(), globalRotation(),
			isModified(true), modelMatrix(), gameObject(gameObject), parent(nullptr), children()
		{

		}

		glm::mat4 Transform::CalculateLocalModelMatrix() const
		{
			glm::mat4 model = glm::mat4(1.0f);
			// first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
			model = glm::translate(model, glm::vec3(localPosition, 0.0f));
			// move origin of rotation to center of quad
			model = glm::translate(model, glm::vec3(0.5f * localScale.x, 0.5f * localScale.y, 0.0f));
			// then rotate
			model = glm::rotate(model, glm::radians(localRotation), glm::vec3(0.0f, 0.0f, 1.0f));
			// move origin back
			model = glm::translate(model, glm::vec3(-0.5f * localScale.x, -0.5f * localScale.y, 0.0f));
			// then rotate
			model = glm::scale(model, glm::vec3(localScale, 1.0f)); // last scale
			return model;
		}

		void Transform::UpdateCachedData() const
		{
			if (isModified)
			{
				if (parent == nullptr)
				{
					modelMatrix = CalculateLocalModelMatrix();

					globalPosition = localPosition;
					globalScale = localScale;
					globalRotation = localRotation;
				}
				else
				{
					modelMatrix = parent->transform.GetModelMatrix() * CalculateLocalModelMatrix();

					globalPosition = glm::vec2(modelMatrix[3][0], modelMatrix[3][1]);

					float scaleX = glm::length(glm::vec2(modelMatrix[0][0], modelMatrix[0][1]));
					float scaleY = glm::length(glm::vec2(modelMatrix[1][0], modelMatrix[1][1]));
					globalScale = glm::vec2(scaleX, scaleY);

					globalRotation = -glm::degrees(atan2(modelMatrix[1][0] / scaleY, modelMatrix[0][0] / scaleX));
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

		bool Transform::IsChild(GameObject* potentialChild) const
		{
			for (GameObject* child : children)
			{
				if (child == potentialChild || child->transform.IsChild(potentialChild))
				{
					return true;
				}
			}
			return false;
		}

		void Transform::RecalcLocalTransform(const glm::mat4& matrix)
		{
			glm::vec2 matrixPos = { matrix[3][0], matrix[3][1] };

			float scaleX = glm::length(glm::vec2(matrix[0][0], matrix[0][1]));
			float scaleY = glm::length(glm::vec2(matrix[1][0], matrix[1][1]));
			localScale = { scaleX, scaleY };

			float rad = atan2(matrix[1][0] / scaleY, matrix[0][0] / scaleX);
			glm::vec2 pivotWorld = 0.5f * localScale;

			glm::mat2 rotMat = {
				{ cos(rad), -sin(rad) },
				{ sin(rad),  cos(rad) }
			};

			glm::vec2 rotatedPivot = rotMat * pivotWorld;

			localPosition = matrixPos - pivotWorld + rotatedPivot;
			localRotation = -glm::degrees(rad);
		}

		void Transform::SetLocalPosition(const glm::vec2& newPosition)
		{
			localPosition = newPosition;
			MarkAsModified();
		}

		void Transform::AddLocalPosition(const glm::vec2& deltaPosition)
		{
			localPosition += deltaPosition;
			MarkAsModified();
		}

		glm::vec2 Transform::GetGlobalPosition() const
		{
			UpdateCachedData();

			return globalPosition;
		}

		void Transform::SetLocalScale(const glm::vec2& newScale)
		{
			localScale = newScale;
			MarkAsModified();
		}

		void Transform::AddLocalScale(const glm::vec2& deltaScale)
		{
			localScale += deltaScale;
			MarkAsModified();
		}

		glm::vec2 Transform::GetGlobalScale() const
		{
			UpdateCachedData();

			return globalScale;
		}

		void Transform::SetLocalRotation(float newRotation)
		{
			localRotation = newRotation;
			MarkAsModified();
		}

		void Transform::AddLocalRotation(float deltaRotation)
		{
			localRotation += deltaRotation;
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

		void Transform::SetParent(GameObject* newParent, bool keepWorldTransform)
		{
			if(gameObject == newParent || IsChild(newParent))
			{
				L2DE_LOG_ERROR("The GameObject can not be a child of itself or its descendants.");
				return;
			}

			ClearParent();

			if (newParent != nullptr)
			{
				if (keepWorldTransform)
				{
					glm::mat4 invParent = glm::inverse(newParent->transform.GetModelMatrix());
					glm::mat4 newLocalMatix = invParent * GetModelMatrix();

					RecalcLocalTransform(newLocalMatix);
				}

				parent = newParent;
				parent->transform.children.push_back(gameObject);
				MarkAsModified();
			}
		}

		void Transform::ClearParent(bool keepWorldTransform)
		{
			if (parent != nullptr)
			{
				if (keepWorldTransform)
				{
					//recalc the local transform from the global model matrix
					RecalcLocalTransform(GetModelMatrix());
				}

				auto& siblings = parent->transform.children;
				siblings.erase(std::remove(siblings.begin(), siblings.end(), gameObject), siblings.end());

				parent = nullptr;
				MarkAsModified();
			}
		}
	}
}