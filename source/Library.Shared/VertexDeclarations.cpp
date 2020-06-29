#include "pch.h"
#include "VertexDeclarations.h"
#include "Mesh.h"
#include "ColorHelper.h"

using namespace std;
using namespace gsl;
using namespace glm;

namespace Library
{
	template <typename T>
	void VertexDeclaration<T>::CreateVertexBuffer(const span<const T>& vertices, GLuint& vertexBuffer)
	{
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, VertexSize() * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	void VertexPosition::CreateVertexBuffer(const Mesh& mesh, GLuint& vertexBuffer)
	{
		const auto& sourceVertices = mesh.Vertices();

		vector<VertexPosition> vertices;
		vertices.reserve(sourceVertices.size());

		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const vec3& position = sourceVertices.at(i);
			vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f));
		}

		VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
	}

	void VertexPositionColor::CreateVertexBuffer(const Mesh& mesh, GLuint& vertexBuffer, bool randomColor)
	{
		const vector<vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPositionColor> vertices;
		vertices.reserve(sourceVertices.size());
		if (mesh.VertexColors().size() > 0)
		{
			auto& vertexColors = mesh.VertexColors().at(0);
			assert(vertexColors.size() == sourceVertices.size());

			for (size_t i = 0; i < sourceVertices.size(); i++)
			{
				const vec3& position = sourceVertices.at(i);
				const vec4& color = vertexColors.at(i);
				vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f), color);
			}
		}
		else
		{
			for (size_t i = 0; i < sourceVertices.size(); i++)
			{
				const vec3& position = sourceVertices.at(i);
				const vec4& color = (randomColor ? ColorHelper::RandomColor() : ColorHelper::White);
				vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f), color);
			}
		}

		VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
	}

	void VertexPositionTexture::CreateVertexBuffer(const Mesh& mesh, GLuint& vertexBuffer)
	{
		const vector<vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPositionTexture> vertices;
		vertices.reserve(sourceVertices.size());

		const auto& textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates.size() == sourceVertices.size());

		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const vec3& position = sourceVertices.at(i);
			const vec2& uv = static_cast<vec2>(textureCoordinates.at(i));
			vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f), uv);
		}

		VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
	}

	void VertexPositionNormal::CreateVertexBuffer(const Mesh& mesh, GLuint& vertexBuffer)
	{
		const vector<vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPositionNormal> vertices;
		vertices.reserve(sourceVertices.size());

		const vector<vec3>& normals = mesh.Normals();
		assert(normals.size() == sourceVertices.size());

		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const vec3& position = sourceVertices.at(i);
			const vec3& normal = normals.at(i);
			vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f), normal);
		}

		VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
	}

	void VertexPositionTextureNormal::CreateVertexBuffer(const Mesh& mesh, GLuint& vertexBuffer)
	{
		const vector<vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPositionTextureNormal> vertices;
		vertices.reserve(sourceVertices.size());

		const auto& textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates.size() == sourceVertices.size());

		const vector<vec3> & normals = mesh.Normals();
		assert(normals.size() == sourceVertices.size());

		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const vec3& position = sourceVertices.at(i);
			const vec2& uv = static_cast<vec2>(textureCoordinates.at(i));
			const vec3& normal = normals.at(i);
			vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f), uv, normal);
		}

		VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
	}

	void VertexPositionTextureNormalTangentBinormal::CreateVertexBuffer(const Mesh& mesh, GLuint& vertexBuffer)
	{
		const vector<vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPositionTextureNormalTangentBinormal> vertices;
		vertices.reserve(sourceVertices.size());

		const auto& textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates.size() == sourceVertices.size());

		const vector<vec3> & normals = mesh.Normals();
		assert(normals.size() == sourceVertices.size());

		const vector<vec3> & tangents = mesh.Tangents();
		assert(tangents.size() == sourceVertices.size());

		const vector<vec3> & binormals = mesh.BiNormals();
		assert(binormals.size() == sourceVertices.size());

		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const vec3& position = sourceVertices.at(i);
			const vec2& uv = static_cast<vec2>(textureCoordinates.at(i));
			const vec3& normal = normals.at(i);
			const vec3& tangent = tangents.at(i);
			const vec3& binormal = binormals.at(i);
			vertices.emplace_back(vec4(position.x, position.y, position.z, 1.0f), uv, normal, tangent, binormal);
		}

		VertexDeclaration::CreateVertexBuffer(vertices, vertexBuffer);
	}
}