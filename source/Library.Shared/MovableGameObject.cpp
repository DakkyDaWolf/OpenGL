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

	vec4 MovableGameObject::Direction() const
	{
		return -1.f * mForward;
	}

	const glm::mat4& MovableGameObject::Rotation() const
	{
		return mRotation;
	}

	const glm::vec4& MovableGameObject::Forward() const
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

	const glm::mat4& MovableGameObject::Transform()
	{
		UpdateTransform();
		return mTransform;
	}

	void MovableGameObject::SetPosition(float x, float y, float z)
	{
		SetPosition(vec3(x, y, z));
	}

	void MovableGameObject::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
		MarkDirty();
	}

	void MovableGameObject::ApplyTranslation(float x, float y, float z)
	{
		ApplyTranslation(vec3(x, y, z));
	}

	void MovableGameObject::ApplyTranslation(const glm::vec3& translation)
	{
		mTransform = translate(mTransform, translation);
		mPosition += translation;
		MarkDirty();
	}

	void MovableGameObject::ApplyScale(float newScale)
	{
		mScale *= newScale;
		MarkDirty();
	}

	void MovableGameObject::SetScale(float newScale)
	{
		mScale = vec3(newScale, newScale, newScale);
		MarkDirty();
	}

	void MovableGameObject::ApplyScale(const vec3& newScale)
	{
		mScale *= newScale;
		MarkDirty();
	}

	void MovableGameObject::SetScale(const vec3& newScale)
	{
		mScale = newScale;
		MarkDirty();
	}

	void MovableGameObject::ApplyRotation(const glm::mat4& transform)
	{
		mRotation = transform * mRotation;
		mForward = mRotation * Vector4Helper::Forward;
		mRight = mRotation * Vector4Helper::Right;
		mUp = mRotation * Vector4Helper::Up;
		MarkDirty();
	}

	void MovableGameObject::MarkDirty()
	{
		for (auto& child : mChildren)
		{
			if (child) child->MarkDirty();
		}
		mTransformDataDirty = true;
	}

	void MovableGameObject::Orphan()
	{
		if (mParent)
		{
			mParent->Disown(*this);
		}
	}

	void MovableGameObject::Adopt(MovableGameObject& newChild)
	{
		if (IsDescendantOf(newChild)) throw std::runtime_error("Cannot adopt your ancestor!");
		if (IsAncestorOf(newChild)) newChild.Orphan();

		newChild.mParent = this;
		mChildren.push_back(&newChild);
	}

	void MovableGameObject::Disown(MovableGameObject& oldChild)
	{
		auto found = std::find(mChildren.begin(), mChildren.end(), &oldChild);
		if (found != mChildren.end())
		{
			mChildren.erase(found);
			oldChild.mParent = nullptr;
		}
	}

	void MovableGameObject::DisownAt(size_t index)
	{
		if (index < mChildren.size())
		{
			mChildren[index]->mParent = nullptr;
			mChildren.erase(mChildren.begin() + index);
		}
	}

	bool MovableGameObject::IsAncestorOf(const MovableGameObject& other) const
	{
		return other.IsDescendantOf(*this);
	}

	bool MovableGameObject::IsDescendantOf(const MovableGameObject& other) const
	{
		return mParent ? (mParent == &other ? true : mParent->IsDescendantOf(other)) : false;
	}

	void MovableGameObject::Update(const GameTime& gameTime)
	{
		GameComponent::Update(gameTime);

		UpdateTransform();
	}

	void MovableGameObject::Rename(const std::string& name)
	{
		mName = name;
	}

	const std::string& MovableGameObject::Name() const
	{
		return mName;
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
		mTransformDataDirty = true;
	}

	void MovableGameObject::ResetPosition()
	{
		mPosition = vec3(0);
		mTransformDataDirty = true;
	}

	void MovableGameObject::ResetScale()
	{
		mScale = vec3(1);
		mTransformDataDirty = true;
	}

	void MovableGameObject::Initialize()
	{
		Reset();
		UpdateTransform();
	}

	void MovableGameObject::UpdateTransform()
	{
		if (mTransformDataDirty)
		{
			mTransform = mRotation * mat4(1);
			mTransform = scale(mTransform, mScale);
			mTransform[3] += vec4(mPosition, 0);
			if (mParent) mTransform = mParent->Transform() * mTransform;
			mTransformDataDirty = false;
		}
	}

}