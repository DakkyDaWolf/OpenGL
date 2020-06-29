#include "pch.h"
#include "TextureProjector.h"

using namespace std;

namespace Library
{

	RTTI_DEFINITIONS(TextureProjector)

	TextureProjector::TextureProjector(Game& game, float fieldOfView, float nearPlaneDistance, float farPlaneDistance, size_t resolution) :
		PerspectiveCamera(game, fieldOfView, 1.f, nearPlaneDistance, farPlaneDistance)
	{
		mDrawTarget = Framebuffer::CreateFramebuffer(GLuint(resolution), GLuint(resolution));
	}


}