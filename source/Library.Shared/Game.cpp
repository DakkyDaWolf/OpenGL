#include "pch.h"
#include "Game.h"
#include "DrawableGameComponent.h"
#include "Utility.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Game)

	Game::Game(const string& windowTitle) :
		mWindowTitle(windowTitle), mScreenWidth(DefaultScreenWidth), mScreenHeight(DefaultScreenHeight)
	{
		GlobalServices.AddService(TypeIdClass(), this);
	}

	void Game::Run()
	{
		sInternalInstance = this;

		InitializeWindow();
		InitializeOpenGL();
		Initialize();

		mGameClock.Reset();

		while (!glfwWindowShouldClose(mWindow))
		{
			mGameClock.UpdateGameTime(mGameTime);
			Update(mGameTime);
			Draw(mGameTime);

			glfwPollEvents();
		}

		Shutdown();
	}

	void Game::Exit()
	{
		glfwSetWindowShouldClose(mWindow, GL_TRUE);
	}

	void Game::Initialize()
	{
		for (auto& component : mComponents)
		{
			component->Initialize();
		}
	}

	void Game::Update(const GameTime& gameTime)
	{
		for (auto& component : mComponents)
		{
			if (component->Enabled())
			{
				component->Update(gameTime);
			}
		}
	}

	void Game::Draw(const GameTime& gameTime)
	{
		for (auto& component : mComponents)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr && drawableGameComponent->Visible())
			{
				drawableGameComponent->Draw(gameTime);
			}
		}
	}

	void Game::AddKeyboardHandler(KeyboardHandler handler)
	{
		mKeyboardHandlers[&handler] = handler;
	}

	void Game::RemoveKeyboardHandler(KeyboardHandler handler)
	{
		mKeyboardHandlers.erase(&handler);
	}

	void Game::InitializeWindow()
	{
		if (!glfwInit())
		{
			throw runtime_error("glfwInit() failed.");
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mWindowTitle.c_str(), (mIsFullScreen ? monitor : nullptr), nullptr);
		if (mWindow == nullptr)
		{
			Shutdown();
			throw runtime_error("glfwCreateWindow() failed.");
		}

		if (mIsFullScreen == false)
		{
			CenterWindow(monitor);
		}
	}

	void Game::InitializeOpenGL()
	{
		glfwMakeContextCurrent(mWindow);
		
		if (gl3wInit() != 0)
		{
			throw runtime_error("gl3wInit() failed.");
		}

		glGetIntegerv(GL_MAJOR_VERSION, &mMajorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &mMinorVersion);

		if (mDepthStencilBufferEnabled)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
		}

		glViewport(0, 0, mScreenWidth, mScreenHeight);

		glfwSetKeyCallback(mWindow, Game::OnKey);
		Begin();
	}

	void Game::Shutdown()
	{
		End();
		mComponents.clear();
		mComponents.shrink_to_fit();
		mKeyboardHandlers.clear();
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	void Game::OnKey(GLFWwindow* /*window*/, int key, int scancode, int action, int mods)
	{
		for (const auto& handler : sInternalInstance->mKeyboardHandlers)
		{
			handler.second(key, scancode, action, mods);
		}
	}

	void Game::CenterWindow(GLFWmonitor* monitor)
	{
		assert(monitor != nullptr);

		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
		if (videoMode == nullptr)
		{
			throw runtime_error("Could not retrieve video mode.");
		}

		int x, y;
		glfwGetMonitorPos(monitor, &x, &y);

		int centerX = x + (videoMode->width - mScreenWidth) / 2;
		int centerY = y + (videoMode->height - mScreenHeight) / 2;

		glfwSetWindowPos(mWindow, centerX, centerY);
	}
}