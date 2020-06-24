#include "pch.h"
#include "RenderingGame.h"
#include "FirstPersonCamera.h"
#include "ColorHelper.h"
#include "Grid.h"
#include "Skybox.h"
#include "EnvironmentMappingDemo.h"

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

		auto skybox = make_shared<Skybox>(*this, camera, "Content/Textures/Maskonaive2_1024/posx.jpg", "Content/Textures/Maskonaive2_1024/negx.jpg", "Content/Textures/Maskonaive2_1024/posy.jpg", "Content/Textures/Maskonaive2_1024/negy.jpg", "Content/Textures/Maskonaive2_1024/posz.jpg", "Content/Textures/Maskonaive2_1024/negz.jpg", 100.0f);
		mComponents.push_back(skybox);

		auto environmentMappingDemo = make_shared<EnvironmentMappingDemo>(*this, camera);
		mComponents.push_back(environmentMappingDemo);
		
		Game::Initialize();

		camera->SetPosition(0, 5, 20);
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