#include "pch.h"
#include "PerspectiveCamera.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(PerspectiveCamera)

	PerspectiveCamera::PerspectiveCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		Camera(game, nearPlaneDistance, farPlaneDistance),
		mFieldOfView(fieldOfView), mAspectRatio(aspectRatio)
	{
	}

	void PerspectiveCamera::Update(const GameTime& gameTime)
	{
		Camera::Update(gameTime);
		if (mProjectionMatrixDataDirty)
		{
			UpdateProjectionMatrix();
		}
	}

	float PerspectiveCamera::AspectRatio() const
	{
		return mAspectRatio;
	}

	void PerspectiveCamera::SetAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
		mProjectionMatrixDataDirty = true;
	}

	float PerspectiveCamera::FieldOfView() const
	{
		return mFieldOfView;
	}

	void PerspectiveCamera::SetFieldOfView(float fieldOfView)
	{
		mFieldOfView = fieldOfView;
		mProjectionMatrixDataDirty = true;
	}

	void PerspectiveCamera::UpdateProjectionMatrix()
	{
		if (mProjectionMatrixDataDirty)
		{
			mProjectionMatrix = perspective(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
		}
	}
}
