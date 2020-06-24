#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "VectorHelper.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(Camera)

	Camera::Camera(Game& game) :
		MovableGameObject(game),
		mNearPlaneDistance(DefaultNearPlaneDistance), mFarPlaneDistance(DefaultFarPlaneDistance)
	{
	}

	Camera::Camera(Game& game, float nearPlaneDistance, float farPlaneDistance) :
		MovableGameObject(game),
		mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance)
	{
	}

	float Camera::NearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	float Camera::FarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}

	const glm::mat4&Camera::ProjectionMatrix()
	{
		if (mProjectionMatrixDataDirty) this->UpdateProjectionMatrix();
		return mProjectionMatrix;
	}

	glm::mat4 Camera::ViewProjectionMatrix()
	{
		return ProjectionMatrix() * ViewMatrix();
	}

	void Camera::Initialize()
	{
		MovableGameObject::Initialize();
		UpdateProjectionMatrix();
	}
}