#include "pch.h"
#include "RenderingGame.h"
#include "FirstPersonCamera.h"
#include "ColorHelper.h"
#include "Grid.h"
#include "PointLightDemo.h"
#include "PlanetSystem.h"
#include "Skybox.h"
#include "DebugTexture.h"

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

		auto skybox = make_shared<Skybox>(*this, camera,
			"Content/Textures/Skybox/right.png",
			"Content/Textures/Skybox/left.png",
			"Content/Textures/Skybox/top.png",
			"Content/Textures/Skybox/bottom.png",
			"Content/Textures/Skybox/front.png",
			"Content/Textures/Skybox/back.png",
			100.0f);
		mComponents.push_back(skybox);

		auto planetSystem = make_shared<PlanetSystem>(*this, camera);
		mComponents.push_back(planetSystem);

		mComponents.push_back(make_shared<DebugTexture>(*this));

		Game::Initialize();

		camera->SetPosition(0, 30, 50);
	}
	
	void RenderingGame::Draw(const GameTime& gameTime)
	{
		static const GLfloat one = 1.0f;

		glClearBufferfv(GL_COLOR, 0, &ColorHelper::Black[0]);
		glClearBufferfv(GL_DEPTH, 0, &one);

		Game::Draw(gameTime);

		glfwSwapBuffers(mWindow);
	}
}