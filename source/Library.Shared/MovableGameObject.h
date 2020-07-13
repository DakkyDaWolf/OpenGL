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
		glm::vec4 Direction() const;
		const glm::mat4& Rotation() const;
		const glm::vec4& Forward() const;
		const glm::vec4& Up() const;
		const glm::vec4& Right() const;
		const glm::mat4& Transform();

		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
		void ApplyTranslation(float x, float y, float z);
		void ApplyTranslation(const glm::vec3& translation);

		void ApplyScale(float newScale);
		void SetScale(float newScale);
		void ApplyScale(const glm::vec3& newScale);
		void SetScale(const glm::vec3& newScale);
		void ApplyRotation(const glm::mat4& transform);

		void MarkDirty();

		void Orphan();
		void Adopt(MovableGameObject& newChild);
		void Disown(MovableGameObject& oldChild);
		void DisownAt(size_t index);
		bool IsAncestorOf(const MovableGameObject& other) const;
		bool IsDescendantOf(const MovableGameObject& other) const;

		virtual void Update(const GameTime& gameTime) override;

		void Rename(const std::string& name);
		const std::string& Name() const;

		virtual void Reset();
		virtual void ResetRotation();
		virtual void ResetPosition();
		virtual void ResetScale();
		virtual void Initialize() override;
		virtual void UpdateTransform();

	protected:
		std::string mName;
		MovableGameObject* mParent{ nullptr };
		std::vector<MovableGameObject*> mChildren;

		glm::vec3 mScale = glm::vec3(1);
		glm::vec3 mPosition = Vector3Helper::Zero;
		glm::mat4 mRotation = glm::mat4(1);
		glm::vec4 mForward = glm::vec4(Vector3Helper::Forward, 0);
		glm::vec4 mUp = glm::vec4(Vector3Helper::Up, 0);
		glm::vec4 mRight = glm::vec4(Vector3Helper::Right, 0);

		glm::mat4 mTransform{ 1 };

		bool mTransformDataDirty{ true };
	};
}

