#include "pch.h"
#include "GraphicsGame.h"
#include "FirstPersonCamera.h"
#include "ColorHelper.h"
#include "Grid.h"
#include "Skybox.h"
#include "ImGuiComponent.h"
#include "SpotLight.h"
#include <glm/gtx/transform.hpp>
#include "GraphicsProgram.h"

using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(GraphicsGame)

	GraphicsGame::GraphicsGame(const string& windowTitle) :
		Game(windowTitle)
	{
		//mIsFullScreen = true;
		mDepthStencilBufferEnabled = true;
	}

	void GraphicsGame::Initialize()
	{
		auto camera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());

		/*auto skybox = make_shared<Skybox>(*this, camera,
			"Content/Textures/Maskonaive2_1024/posx.jpg",
			"Content/Textures/Maskonaive2_1024/negx.jpg",
			"Content/Textures/Maskonaive2_1024/posy.jpg",
			"Content/Textures/Maskonaive2_1024/negy.jpg",
			"Content/Textures/Maskonaive2_1024/posz.jpg",
			"Content/Textures/Maskonaive2_1024/negz.jpg", 100.0f);
		mComponents.push_back(skybox);*/

		//auto grid = make_shared<Grid>(*this, camera);
		//mComponents.push_back(grid);

		auto graphicsProgram = make_shared<GraphicsProgram>(*this, camera);
		mComponents.push_back(graphicsProgram);
		
		AddKeyboardHandler([&](int key, int /*scancode*/, int action, int /*mods*/) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				Exit();
			}

			if (key == GLFW_KEY_K && action == GLFW_PRESS)
			{
				//skybox->SetVisible(!skybox->Visible());
			}
		});

		Game::Initialize();

		camera->SetPosition(0, 5, 40);
	}

	void GraphicsGame::Draw(const GameTime& gameTime)
	{
		static const GLfloat one = 1.0f;

		glClearBufferfv(GL_COLOR, 0, &ColorHelper::Black[0]);
		glClearBufferfv(GL_DEPTH, 0, &one);

		Game::Draw(gameTime);

		glfwSwapBuffers(mWindow);
	}
}