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

	void Camera::SetNearPlaneDistance(float nearPlaneDistance)
	{
		mNearPlaneDistance = nearPlaneDistance;
		mProjectionMatrixDataDirty = true;
	}

	void Camera::SetFarPlaneDistance(float farPlaneDistance)
	{
		mFarPlaneDistance = farPlaneDistance;
		mProjectionMatrixDataDirty = true;
	}

	float Camera::NearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	float Camera::FarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}

	const glm::mat4& Camera::ViewMatrix()
	{
		if (mTransformDataDirty) UpdateTransform();
		return mViewMatrix;
	}

	const glm::mat4& Camera::ProjectionMatrix()
	{
		if (mProjectionMatrixDataDirty) UpdateProjectionMatrix();
		return mProjectionMatrix;
	}

	glm::mat4 Camera::ViewProjectionMatrix()
	{
		return ProjectionMatrix() * ViewMatrix();
	}

	//void Camera::ApplyRotation(const glm::mat4& rotation)
	//{
	//	mForward = vec4(normalize(vec3(rotation * mForward)), 0);
	//	mRotation = lookAt(mPosition, mPosition + vec3(mForward), Vector3Helper::Up);

	//	mRight = vec4(normalize(vec3(rotation * Vector4Helper::Right)), 0);
	//	mUp = vec4(normalize(vec3(rotation * Vector4Helper::Up)), 0);

	//	mViewMatrixDataDirty = true;

	//	vec4 direction = rotation * mForward;
	//	//rotate forward direction

	//	mForward = vec4(vec3(normalize(direction)), 0);
	//	//normalize vec4??

	//	vec4 up	= rotation * mUp;
	//	//rotate up, ok
	//	mUp = vec4(vec3(normalize(up)), 0);
	//	//normalize..?

	//	mRight = vec4(cross(vec3(mForward), vec3(mUp)), 0);
	//	//right is cross of fwd and up, yeah
	//	mUp = vec4(cross(vec3(mRight), vec3(mForward)), 0);
	//	mViewMatrixDataDirty = true;
	//}

	void Camera::Update(const GameTime& gameTime)
	{
		GameComponent::Update(gameTime);

		if (mTransformDataDirty) UpdateTransform();
		if (mProjectionMatrixDataDirty) UpdateProjectionMatrix();
	}

	void Camera::Reset()
	{
		MovableGameObject::Reset();
		mTransformDataDirty = true;
	}

	void Camera::Initialize()
	{
		Reset();
	}

	void Camera::UpdateTransform()
	{
		MovableGameObject::UpdateTransform();
		mViewMatrix = lookAt(mPosition, mPosition + vec3(mForward), vec3(mUp));
	}
}