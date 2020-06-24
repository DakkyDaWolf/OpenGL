#include "pch.h"
#include "Texture.h"

namespace Library
{
	RTTI_DEFINITIONS(Texture)

		Texture::Texture(const GLuint shaderResource) :
		mShaderResource(shaderResource)
	{
	}
}