#pragma once

#include "Game.h"

namespace Library
{
	class GameTime;
	class FirstPersonCamera;
	class Grid;
}

namespace Rendering
{
	class BlinnPhongDemo;

	class RenderingGame final : public Library::Game
	{
		RTTI_DECLARATIONS(RenderingGame, Game)

	public:
		RenderingGame(const std::string& windowTitle);

	protected:
		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		std::shared_ptr<Library::FirstPersonCamera> mCamera;
		std::shared_ptr<Library::Grid> mGrid;
		std::shared_ptr<BlinnPhongDemo> mBlinnPhongDemo;
	};
}
