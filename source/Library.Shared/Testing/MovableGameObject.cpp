#include "pch.h"
#include "MovableGameObject.h"

using namespace glm;

namespace Library
{

	RTTI_DEFINITIONS(MovableGameObject)

	MovableGameObject::MovableGameObject(Game& game) :
		GameComponent(game)
	{
	}

	const vec3& MovableGameObject::Position() const
	{
		return mPosition;
	}

	const vec3& MovableGameObject::Direction() const
	{
		return mDirection;
	}

	const vec3& MovableGameObject::Up() const
	{
		return mUp;
	}

	const vec3& MovableGameObject::Right() const
	{
		return mRight;
	}

	const mat4& MovableGameObject::ViewMatrix()
	{
		if (mViewMatrixDataDirty) UpdateViewMatrix();
		return mViewMatrix;
	}

	void MovableGameObject::SetPosition(float x, float y, float z)
	{
		mPosition = vec3(x, y, z);
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::ApplyScale(float newScale)
	{
		mViewMatrix = scale(mViewMatrix, glm::vec3(newScale, newScale, newScale));
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::ApplyRotation(const glm::mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = (vec3)normalize(direction);

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = (vec3)normalize(up);

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::Update(const GameTime& gameTime)
	{
		GameComponent::Update(gameTime);
		UpdateViewMatrix();
	}

	void MovableGameObject::Reset()
	{
		mPosition = Vector3Helper::Zero;
		mDirection = Vector3Helper::Forward;
		mUp = Vector3Helper::Up;
		mRight = Vector3Helper::Right;
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::Initialize()
	{
		Reset();
		UpdateViewMatrix();
	}

	void MovableGameObject::UpdateViewMatrix()
	{
		if (mViewMatrixDataDirty)
		{
			vec3 target = mPosition + mDirection;
			mViewMatrix = lookAt(mPosition, target, mUp);
			mViewMatrixDataDirty = false;
		}
	}

}