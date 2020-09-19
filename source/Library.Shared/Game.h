#pragma once

#include <vector>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <GLFW/glfw3.h>
#include "RTTI.h"
#include "GameClock.h"
#include "GameTime.h"
#include "GameComponent.h"
#include "ServiceContainer.h"
#include "RenderTarget.h"

namespace Library
{
	class Game : public RenderTarget
	{
		RTTI_DECLARATIONS(Game, RenderTarget)

	public:
		typedef std::function<void(int, int, int, int)> KeyboardHandler;
		
		Game(const std::string& windowTitle);
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;
		virtual ~Game() = default;

		GLFWwindow* Window() const;

		bool DepthStencilBufferEnabled() const;
		const std::string& WindowTitle() const;
		int ScreenWidth() const;
		int ScreenHeight() const;
		float AspectRatio() const;
		bool IsFullScreen() const;
		const std::vector<std::shared_ptr<GameComponent>>& Components() const;
		const ServiceContainer& Services() const;

		virtual void Run();
		virtual void Exit();
		virtual void Initialize();
		virtual void Update(const GameTime& gameTime);
		virtual void Draw(const GameTime& gameTime);

		void AddKeyboardHandler(KeyboardHandler handler);
		void RemoveKeyboardHandler(KeyboardHandler handler);

		GameTime& Time() { return mGameTime; }

	protected:
		virtual void InitializeWindow();
		virtual void InitializeOpenGL();
		virtual void Shutdown();

		inline static const std::uint32_t DefaultScreenWidth = 1920;
		inline static const std::uint32_t DefaultScreenHeight = 1080;

		std::string mWindowTitle;		
		GLFWwindow* mWindow = nullptr;
		std::uint32_t mScreenWidth;
		std::uint32_t mScreenHeight;
		bool mIsFullScreen = false;
		GLint mMajorVersion = 0;
		GLint mMinorVersion = 0;
		bool mDepthStencilBufferEnabled = false;
		GameClock mGameClock;
		GameTime mGameTime;
		std::vector<std::shared_ptr<GameComponent>> mComponents;
		ServiceContainer mServices;
		std::map<KeyboardHandler*, KeyboardHandler> mKeyboardHandlers;

	private:
		inline static Game* sInternalInstance = nullptr;

		static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);

		void CenterWindow(GLFWmonitor* monitor);
	};
}

#include "Game.inl"