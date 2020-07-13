#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "GameComponent.h"
#include "MovableGameObject.h"

namespace Library
{
	class Game;
	class GameTime;

	class Camera : public MovableGameObject
	{
		RTTI_DECLARATIONS(Camera, MovableGameObject)

	public:
		Camera(Game& game);
		Camera(Game& game, float nearPlaneDistance, float farPlaneDistance);
		Camera(const Camera&) = default;
		Camera& operator=(const Camera&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(Camera&&) = default;
		virtual ~Camera() = default;

		void SetNearPlaneDistance(float nearPlaneDistance);
		void SetFarPlaneDistance(float farPlaneDistance);

		float NearPlaneDistance() const;
		float FarPlaneDistance() const;

		const glm::mat4& ViewMatrix();
		const glm::mat4& ProjectionMatrix();
		glm::mat4 ViewProjectionMatrix();

		//virtual void ApplyRotation(const glm::mat4& rotation) override;

		virtual void Update(const GameTime& gameTime) override;
		virtual void Reset() override;
		virtual void Initialize() override;

		virtual void UpdateProjectionMatrix() { mProjectionMatrixDataDirty = false; }
		virtual void UpdateTransform() override;

		inline static const float DefaultNearPlaneDistance{ 0.01f };
		inline static const float DefaultFarPlaneDistance{ 1000.0f };

	protected:
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		glm::vec3 mDirection;

		glm::mat4 mProjectionMatrix{ 1 };
		glm::mat4 mViewMatrix{ 1 };

		bool mProjectionMatrixDataDirty{ true };
	};
}

