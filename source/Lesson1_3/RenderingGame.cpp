#include "pch.h"
#include "RenderingGame.h"
#include "FirstPersonCamera.h"
#include "ColorHelper.h"
#include "PointDemo.h"

using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(RenderingGame)

	RenderingGame::RenderingGame(const string& windowTitle) :
		Game(windowTitle)
	{
	}

	void RenderingGame::Initialize()
	{
		auto camera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());

		AddKeyboardHandler([&](int key, int /*scancode*/, int action, int /*mods*/) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				Exit();
			}
		});

		auto pointDemo =  make_shared<PointDemo>(*this, camera);
		mComponents.push_back(pointDemo);

		Game::Initialize();
	}

	void RenderingGame::Draw(const GameTime& gameTime)
	{
		glClearBufferfv(GL_COLOR, 0, &ColorHelper::CornflowerBlue[0]);

		Game::Draw(gameTime);

		glfwSwapBuffers(mWindow);
	}
}