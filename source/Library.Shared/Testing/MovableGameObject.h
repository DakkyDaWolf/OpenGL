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
		const glm::vec3& Direction() const;
		const glm::vec3& Up() const;
		const glm::vec3& Right() const;

		const glm::mat4& ViewMatrix();

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(const glm::vec3& position);
		void ApplyScale(float newScale);
		virtual void ApplyRotation(const glm::mat4& transform);

		virtual void Update(const GameTime& gameTime) override;

		virtual void Reset();
		virtual void Initialize() override;
		virtual void UpdateViewMatrix();

	protected:

		glm::vec3 mPosition = Vector3Helper::Zero;
		glm::vec3 mDirection = Vector3Helper::Forward;
		glm::vec3 mUp = Vector3Helper::Up;
		glm::vec3 mRight = Vector3Helper::Right;

		glm::mat4 mViewMatrix{ 1 };

		bool mViewMatrixDataDirty{ true };
	};
}

