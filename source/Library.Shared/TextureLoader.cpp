#include "pch.h"
#include "TextureLoader.h"

#include <SOIL/SOIL.h>

namespace Library
{

	GLuint TextureLoader::LoadTexture(const std::string& fileName)
	{
		return SOIL_load_OGL_texture(fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	}

	GLuint TextureLoader::LoadCubemapSingleTexture(const std::string& /*fileName*/)
	{
		return 0;
	}

	GLuint TextureLoader::LoadCubemapMultiTexture(const std::string& px, const std::string& nx, const std::string& py, const std::string& ny, const std::string& pz, const std::string& nz)
	{
		return SOIL_load_OGL_cubemap(px.c_str(), nx.c_str(), py.c_str(), ny.c_str(), pz.c_str(), nz.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	}

	void TextureLoader::DeleteTexture(GLuint textureName)
	{
		glDeleteTextures(1, &textureName);
	}

}