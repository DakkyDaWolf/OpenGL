#pragma once

#include "Mesh.h"
#include <GL/gl3w.h>

#define VAOVertexPos	0;	// B00000 (assuming positions are always included, so flag isn't necessary)
#define VAOTexCoords	1;	// B00001
#define VAONormals		2;	// B00010
#define VAOTangents		4;	// B00100
#define VAOBinormals	8;	// B01000
#define VAOVertColors	16;	// B10000

namespace Library
{

	class MeshResource
	{
	private:

		const Mesh& InnerMesh;
		GLuint DiffuseTexture{ 0 };

		GLuint VertexBufferObjects[32] { 0 };
		GLuint StoredIndexBuffer{ 0 };

		GLuint VertexAttributes{ 0 };



		inline bool HasVertexAttribs(GLuint flaggedAttributes) const
		{
			return !(flaggedAttributes ^ (flaggedAttributes & VertexAttributes));
		}

	public:

		MeshResource() = delete;
		MeshResource(const Mesh& sourceMesh, const std::string& textureFilename = "");
		MeshResource(const MeshResource& rhs) = delete;
		MeshResource(MeshResource&& rhs) noexcept = default;
		MeshResource& operator=(const MeshResource& rhs) = delete;
		MeshResource& operator=(MeshResource&& rhs) noexcept = default;
		~MeshResource();

		GLuint Diffuse() const;
		GLuint IndexBuffer() const;
		size_t IndexCount() const;
		GLuint VBO(GLuint flags);

		const Mesh& GetMesh() const;

		const std::string& Name() const;

		bool SetDiffuseTexture(const std::string& textureFilename);

		const std::vector<glm::vec3>& Vertices() const;
		const std::vector<glm::vec3>& Normals() const;
		const std::vector<glm::vec3>& Tangents() const;
		const std::vector<glm::vec3>& BiNormals() const;
		const std::vector<std::vector<glm::vec3>>& TextureCoordinates() const;
		const std::vector<std::vector<glm::vec4>>& VertexColors() const;
		std::uint32_t FaceCount() const;
		const std::vector<std::uint32_t>& Indices() const;

	};

}