#include "pch.h"
#include "RenderingGame.h"
#include "FirstPersonCamera.h"
#include "VectorHelper.h"
#include "ColorHelper.h"
#include "Grid.h"
#include "CubeDemo.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(RenderingGame)

	RenderingGame::RenderingGame(const string& windowTitle) :
		Game(windowTitle)
	{
		mDepthStencilBufferEnabled = true;
	}

	void RenderingGame::Initialize()
	{
		auto camera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());

		auto grid = make_shared<Grid>(*this, camera);
		mComponents.push_back(grid);

		AddKeyboardHandler([&, grid](int key, int /*scancode*/, int action, int /*mods*/) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				Exit();
			}

			if (key == GLFW_KEY_G && action == GLFW_PRESS)
			{
				grid->SetVisible(!grid->Visible());
			}
		});

		auto cubeDemo = make_shared<CubeDemo>(*this, camera);
		mComponents.push_back(cubeDemo);

		Game::Initialize();

		camera->SetPosition(0, 5, 10);
		camera->ApplyRotation(rotate(mat4(1), radians(30.0f), Vector3Helper::Left));
	}

	void RenderingGame::Draw(const GameTime& gameTime)
	{
		static const GLfloat one = 1.0f;

		glClearBufferfv(GL_COLOR, 0, &ColorHelper::CornflowerBlue[0]);
		glClearBufferfv(GL_DEPTH, 0, &one);

		Game::Draw(gameTime);

		glfwSwapBuffers(mWindow);
	}
}