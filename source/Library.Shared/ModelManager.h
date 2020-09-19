#pragma once

#include "Model.h"
#include "Mesh.h"
#include <unordered_map>
#include <vector>
#include "MeshResource.h"

namespace Library
{

	class ModelManager
	{
	private:

		/// <summary>
		/// Container to store GL texture names, keyed to their respective fileNames
		/// </summary>
		inline static std::unordered_map<std::string, std::pair<Model, std::vector<MeshResource>>> RegisteredModels{};

		/// <summary>
		/// Load a model into memory and create the MeshResource objects from it
		/// </summary>
		/// <param name="fileName">name of the file to be loaded</param>
		static std::pair<Model, std::vector<MeshResource>>& LoadModel(const std::string& fileName);

	public:

		inline static size_t ModelsLoaded{ 0 };
		
		/// <summary>
		/// Return the GL texture name associated with the given fileName
		/// </summary>
		/// <param name="fileName">name of the file whose name is to be accessed</param>
		/// <returns>GL texture name of the registered texture, 0 if not registered</returns>
		static Model& GetModel(const std::string& fileName);

		/// <summary>
		/// Return the GL texture name associated with the given fileName
		/// </summary>
		/// <param name="fileName">name of the file whose name is to be accessed</param>
		/// <returns>GL texture name of the registered texture, 0 if not registered</returns>
		static MeshResource& GetMesh(const std::string& fileName, size_t index = 0);

		/// <summary>
		/// Return the GL texture name associated with the given fileName
		/// </summary>
		/// <param name="fileName">name of the file whose name is to be accessed</param>
		/// <returns>GL texture name of the registered texture, 0 if not registered</returns>
		static std::vector<MeshResource>& GetMeshes(const std::string& fileName);
	};

}