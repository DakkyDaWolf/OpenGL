#include "pch.h"
#include "Texture.h"
#include "TextureManager.h"

namespace Library
{
	RTTI_DEFINITIONS(Texture)

	Texture::Texture(const std::string& fileName)
	{
		mShaderResource = TextureManager::RegisterTexture(fileName);
		if (mShaderResource == 0)
		{
			throw std::runtime_error("SOIL_load_OGL_texture() failed.");
		}
	}

	Texture::Texture(const GLuint shaderResource) :
		mShaderResource(shaderResource)
	{
	}
}