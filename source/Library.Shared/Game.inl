#pragma once

namespace Library
{
	inline GLFWwindow* Game::Window() const
	{
		return mWindow;
	}

	inline bool Game::DepthStencilBufferEnabled() const
	{
		return mDepthStencilBufferEnabled;
	}

	inline const std::string& Game::WindowTitle() const
	{
		return mWindowTitle;
	}

	inline int Game::ScreenWidth() const
	{
		return mScreenWidth;
	}

	inline int Game::ScreenHeight() const
	{
		return mScreenHeight;
	}

	inline float Game::AspectRatio() const
	{
		return static_cast<float>(mScreenWidth) / mScreenHeight;
	}

	inline bool Game::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	inline const std::vector<std::shared_ptr<GameComponent>>& Game::Components() const
	{
		return mComponents;
	}

	inline const ServiceContainer& Game::Services() const
	{
		return mServices;
	}
}