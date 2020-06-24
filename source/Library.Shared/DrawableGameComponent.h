#pragma once

#include <memory>
#include "GameComponent.h"

namespace Library
{
	class Camera;

	class DrawableGameComponent : public GameComponent
	{
		RTTI_DECLARATIONS(DrawableGameComponent, GameComponent)

	public:
		DrawableGameComponent() = default;
		explicit DrawableGameComponent(Game& game, std::shared_ptr<Camera> camera = nullptr);
		DrawableGameComponent(const DrawableGameComponent&) = default;
		DrawableGameComponent& operator=(const DrawableGameComponent&) = default;
		DrawableGameComponent(DrawableGameComponent&&) = default;
		DrawableGameComponent& operator=(DrawableGameComponent&&) = default;
		virtual ~DrawableGameComponent() = default;

		bool Visible() const;
		void SetVisible(bool visible);

		std::shared_ptr<Camera> GetCamera() const;
		void SetCamera(std::shared_ptr<Camera> camera);

		virtual void Draw(const GameTime& gameTime) = 0;

	protected:
		bool mVisible = true;
		std::shared_ptr<Camera> mCamera;
	};
}

#include "DrawableGameComponent.inl"