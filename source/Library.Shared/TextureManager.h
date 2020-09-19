#pragma once

#include <unordered_map>

namespace Library
{

	class TextureManager
	{
	private:

		/// <summary>
		/// Container to store GL texture names, keyed to their respective fileNames
		/// </summary>
		inline static std::unordered_map<std::string, GLuint> RegisteredTextures{};

	public:

		inline static size_t TexturesLoaded{ 0 };

		/// <summary>
		/// Remove all entries in the registry, deleting the textures from OpenGL
		/// </summary>
		~TextureManager();

		/// <summary>
		/// Load a texture into the registry using the TextureLoader
		/// For existing textures, will simply return the registered GL texture name
		/// </summary>
		/// <param name="fileName">name of the file to be registered</param>
		/// <returns>GL texture name of the registered texture, 0 if loading failed</returns>
		static GLuint RegisterTexture(const std::string& fileName);

		static GLuint RegisterCubemapTextures(const std::string& px, const std::string& nx, const std::string& py, const std::string& ny, const std::string& pz, const std::string& nz);

		/// <summary>
		/// Return the GL texture name associated with the given fileName
		/// </summary>
		/// <param name="fileName">name of the file whose name is to be accessed</param>
		/// <returns>GL texture name of the registered texture, 0 if not registered</returns>
		static GLuint GetTexture(const std::string& fileName);

		/// <summary>
		/// Remove the given texture from the registry & delete from GL
		/// </summary>
		/// <param name="fileName">name of the file to remove from the registry</param>
		/// <returns>true if removed, false if not present</returns>
		static bool DeregisterTexture(const std::string& fileName);

	};

}