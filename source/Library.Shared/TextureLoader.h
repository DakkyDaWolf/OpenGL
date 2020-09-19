#pragma once

namespace Library
{

	class TextureLoader
	{
		friend class TextureManager;

	private:
		/// <summary>
		/// Use SOIL to generate a GL texture using a specified input file
		/// </summary>
		/// <param name="fileName">file to create a texture from</param>
		/// <returns>GL texture name on success, 0 on failure</returns>
		static GLuint LoadTexture(const std::string& fileName);

		/// <summary>
		/// Use SOIL to generate a GL cubemap texture using a specified input file
		/// </summary>
		/// <param name="fileName">file to create a texture from</param>
		/// <returns>GL texture name on success, 0 on failure</returns>
		static GLuint LoadCubemapSingleTexture(const std::string& fileName);

		static GLuint LoadCubemapMultiTexture(const std::string& px, const std::string& nx, const std::string& py, const std::string& ny, const std::string& pz, const std::string& nz);

		/// <summary>
		/// Remove the given texture from OpenGL state machine
		/// </summary>
		/// <param name="textureName">GL texture name associated with the desired texture to delete</param>
		static void DeleteTexture(GLuint textureName);
	};

}