#pragma once 

namespace Library
{
	inline bool DrawableGameComponent::Visible() const
	{
		return mVisible;
	}

	inline void DrawableGameComponent::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	inline std::shared_ptr<Camera> DrawableGameComponent::GetCamera() const
	{
		return mCamera;
	}

	inline void DrawableGameComponent::SetCamera(std::shared_ptr<Camera> camera)
	{
		mCamera = std::move(camera);
	}
}