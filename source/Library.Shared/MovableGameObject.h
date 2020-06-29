#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "RTTI.h"
#include "VectorHelper.h"
#include "GameComponent.h"

namespace Library
{
	class Game;
	class GameTime;

	class MovableGameObject : public GameComponent
	{
		RTTI_DECLARATIONS(MovableGameObject, GameComponent)

	public:
		MovableGameObject(Game& game);
		MovableGameObject(const MovableGameObject&) = default;
		MovableGameObject& operator=(const MovableGameObject&) = default;
		MovableGameObject(MovableGameObject&&) = default;
		MovableGameObject& operator=(MovableGameObject&&) = default;
		virtual ~MovableGameObject() = default;

		const glm::vec3& Position() const;
		const glm::vec4& Direction() const;
		const glm::vec4& Up() const;
		const glm::vec4& Right() const;

		const glm::mat4& ViewMatrix();
		const glm::mat4& Transform();

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(const glm::vec3& position);
		virtual void ApplyTranslation(float x, float y, float z);
		virtual void ApplyTranslation(const glm::vec3& translation);
		void ApplyScale(float newScale);
		void SetScale(float newScale);
		void ApplyScale(const glm::vec3& newScale);
		void SetScale(const glm::vec3& newScale);
		virtual void ApplyRotation(const glm::mat4& transform);

		virtual void Update(const GameTime& gameTime) override;

		virtual void Reset();
		virtual void ResetRotation();
		virtual void ResetPosition();
		virtual void ResetScale();
		virtual void Initialize() override;
		virtual void UpdateViewMatrix();

	protected:

		glm::vec3 mScale = glm::vec3(1);
		glm::vec3 mPosition = Vector3Helper::Zero;
		glm::mat4 mRotation = glm::mat4(1);
		glm::vec4 mForward = glm::vec4(Vector3Helper::Forward, 0);
		glm::vec4 mUp = glm::vec4(Vector3Helper::Up, 0);
		glm::vec4 mRight = glm::vec4(Vector3Helper::Right, 0);

		glm::mat4 mViewMatrix{ 1 };
		glm::mat4 mTransform{ 1 };

		bool mTransformDirty{ true };
		bool mViewMatrixDataDirty{ true };
	};
}

