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

	const vec4& MovableGameObject::Direction() const
	{
		return mForward;
	}

	const vec4& MovableGameObject::Up() const
	{
		return mUp;
	}

	const vec4& MovableGameObject::Right() const
	{
		return mRight;
	}

	const mat4& MovableGameObject::ViewMatrix()
	{
		if (mViewMatrixDataDirty) UpdateViewMatrix();
		return mViewMatrix;
	}

	const glm::mat4& MovableGameObject::Transform()
	{
		return ViewMatrix();
	}

	void MovableGameObject::SetPosition(float x, float y, float z)
	{
		SetPosition(vec3(x, y, z));
	}

	void MovableGameObject::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::ApplyTranslation(float x, float y, float z)
	{
		ApplyTranslation(vec3(x, y, z));
	}

	void MovableGameObject::ApplyTranslation(const glm::vec3& translation)
	{
		mViewMatrix = translate(mViewMatrix, translation);
		mPosition += translation;
	}

	void MovableGameObject::ApplyScale(float newScale)
	{
		mScale *= newScale;
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::SetScale(float newScale)
	{
		mScale = vec3(newScale, newScale, newScale);
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::ApplyScale(const vec3& newScale)
	{
		mScale *= newScale;
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::SetScale(const vec3& newScale)
	{
		mScale = newScale;
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::ApplyRotation(const glm::mat4& transform)
	{
		mRotation = transform * mRotation;
		mForward = normalize(mRotation * Vector4Helper::Forward);
		mRight = normalize(mRotation * Vector4Helper::Right);
		mUp = normalize(mRotation * Vector4Helper::Up);
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::Update(const GameTime& gameTime)
	{
		GameComponent::Update(gameTime);
		UpdateViewMatrix();
	}

	void MovableGameObject::Reset()
	{
		ResetPosition();
		ResetRotation();
		ResetScale();
	}

	void MovableGameObject::ResetRotation()
	{
		mRotation = mat4(1);
		mForward = Vector4Helper::Forward;
		mRight = Vector4Helper::Right;
		mUp = Vector4Helper::Up;
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::ResetPosition()
	{
		mPosition = vec3(0);
		mViewMatrixDataDirty = true;
	}

	void MovableGameObject::ResetScale()
	{
		mScale = vec3(1);
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
			mViewMatrix = mRotation * mat4(1);
			mViewMatrix = scale(mViewMatrix, mScale);
			mViewMatrix[3] += vec4(mPosition, 0);
			mViewMatrixDataDirty = false;
		}
	}

}