#include "pch.h"
#include "TextureManager.h"
#include "TextureLoader.h"

namespace Library
{

	TextureManager::~TextureManager()
	{
		for (const auto textureName : RegisteredTextures)
		{
			TextureLoader::DeleteTexture(textureName.second);
		}
	}

	GLuint TextureManager::RegisterTexture(const std::string& fileName)
	{
		if (fileName.empty()) return 0;
		if (RegisteredTextures.count(fileName)) return RegisteredTextures.at(fileName);

		GLuint loadedTexture = TextureLoader::LoadTexture(fileName);

		if (loadedTexture) RegisteredTextures.emplace(std::make_pair(fileName, loadedTexture));

		++TexturesLoaded;

		return loadedTexture;
	}

	GLuint TextureManager::RegisterCubemapTextures(const std::string& px, const std::string& nx, const std::string& py, const std::string& ny, const std::string& pz, const std::string& nz)
	{
		std::string assembledKey = px;
		assembledKey.append(nx).append(py).append(ny).append(pz).append(nz);

		if (RegisteredTextures.count(assembledKey)) return RegisteredTextures.at(assembledKey);

		GLuint loadedTexture = TextureLoader::LoadCubemapMultiTexture(px, nx, py, ny, pz, nz);

		if (loadedTexture) RegisteredTextures.emplace(std::make_pair(assembledKey, loadedTexture));

		++TexturesLoaded;

		return loadedTexture;
	}

	GLuint TextureManager::GetTexture(const std::string& fileName)
	{
		return RegisteredTextures.count(fileName) ? RegisteredTextures.at(fileName) : 0;
	}

	bool TextureManager::DeregisterTexture(const std::string& fileName)
	{
		if (!RegisteredTextures.count(fileName)) return false;
		
		TextureLoader::DeleteTexture(RegisteredTextures.at(fileName));
		RegisteredTextures.erase(fileName);

		return true;
	}

}