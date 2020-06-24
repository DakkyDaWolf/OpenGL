#include "pch.h"
#include "RenderingGame.h"
#include "ColorHelper.h"

using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(RenderingGame)

	RenderingGame::RenderingGame(const std::string& windowTitle) :
		Game(windowTitle)
	{
	}

	void RenderingGame::Draw(const GameTime& gameTime)
	{
		glClearBufferfv(GL_COLOR, 0, &ColorHelper::CornflowerBlue[0]);

		Game::Draw(gameTime);

		glfwSwapBuffers(mWindow);
	}
}