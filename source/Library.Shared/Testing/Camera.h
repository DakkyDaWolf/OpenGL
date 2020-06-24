#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "MovableGameObject.h"
#include "VectorHelper.h"

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

		float NearPlaneDistance() const;
		float FarPlaneDistance() const;

		const glm::mat4& ProjectionMatrix();
		glm::mat4 ViewProjectionMatrix();

		virtual void Initialize() override;
		virtual void UpdateProjectionMatrix() = 0;

		inline static const float DefaultNearPlaneDistance{ 0.01f };
		inline static const float DefaultFarPlaneDistance{ 1000.0f };

	protected:		
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		glm::mat4 mProjectionMatrix{ 1 };

		bool mViewMatrixDataDirty{ true };
		bool mProjectionMatrixDataDirty{ true };
	};
}

