#pragma once

#include "Game.h"

namespace Rendering
{
	class GraphicsGame final : public Library::Game
	{
		RTTI_DECLARATIONS(GraphicsGame, Library::Game)

	public:
		GraphicsGame(const std::string& windowTitle);

	protected:
		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;
	};
}
