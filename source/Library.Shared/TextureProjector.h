#pragma once
#include "PerspectiveCamera.h"
#include "RenderTarget.h"

namespace Library
{

	class TextureProjector : public PerspectiveCamera, public RenderTarget
	{
		RTTI_DECLARATIONS(TextureProjector, PerspectiveCamera)

	public:

		explicit TextureProjector(Game& game, float fieldOfView = DefaultFieldOfView, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance, size_t resolution = 512);
		TextureProjector(const TextureProjector&) = default;
		TextureProjector& operator=(const TextureProjector&) = default;
		TextureProjector(TextureProjector&&) = default;
		TextureProjector& operator=(TextureProjector&&) = default;
		virtual ~TextureProjector() = default;
	};

}