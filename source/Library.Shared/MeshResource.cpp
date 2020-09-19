#include "pch.h"
#include "MeshResource.h"
#include "TextureManager.h"

namespace Library
{

    const std::string& MeshResource::Name() const
    {
        return InnerMesh.Name();
    }

    bool MeshResource::SetDiffuseTexture(const std::string& textureFilename)
    {
        DiffuseTexture = TextureManager::RegisterTexture(textureFilename);
        return DiffuseTexture;
    }

    const std::vector<glm::vec3>& MeshResource::Vertices() const
    {
        return InnerMesh.Vertices();
    }

    const std::vector<glm::vec3>& MeshResource::Normals() const
    {
        return InnerMesh.Normals();
    }

    const std::vector<glm::vec3>& MeshResource::Tangents() const
    {
        return InnerMesh.Tangents();
    }

    const std::vector<glm::vec3>& MeshResource::BiNormals() const
    {
        return InnerMesh.BiNormals();
    }

    const std::vector<std::vector<glm::vec3>>& MeshResource::TextureCoordinates() const
    {
        return InnerMesh.TextureCoordinates();
    }

    const std::vector<std::vector<glm::vec4>>& MeshResource::VertexColors() const
    {
        return InnerMesh.VertexColors();
    }

    std::uint32_t MeshResource::FaceCount() const
    {
        return InnerMesh.FaceCount();
    }

    const std::vector<std::uint32_t>& MeshResource::Indices() const
    {
        return InnerMesh.Indices();
    }

    MeshResource::MeshResource(const Mesh& sourceMesh, const std::string& textureFilename) :
        InnerMesh(sourceMesh), DiffuseTexture(TextureManager::RegisterTexture(textureFilename))
    {
        const_cast<Mesh&>(sourceMesh).CreateIndexBuffer(StoredIndexBuffer);

        if (sourceMesh.TextureCoordinates().size()) VertexAttributes |= VAOTexCoords;
        if (sourceMesh.Normals().size()) VertexAttributes |= VAONormals;
        if (sourceMesh.Tangents().size()) VertexAttributes |= VAOTangents;
        if (sourceMesh.BiNormals().size()) VertexAttributes |= VAOBinormals;
        if (sourceMesh.VertexColors().size()) VertexAttributes |= VAOVertColors;


    }

    MeshResource::~MeshResource()
    {
    }

    GLuint MeshResource::Diffuse() const
    {
        return DiffuseTexture;
    }

    GLuint MeshResource::IndexBuffer() const
    {
        return StoredIndexBuffer;
    }

    size_t MeshResource::IndexCount() const
    {
        return Indices().size();
    }

    GLuint MeshResource::VBO(GLuint flags)
    {
        if (!HasVertexAttribs(flags)) return 0;

        return 0;
    }

    const Mesh& MeshResource::GetMesh() const
    {
        return InnerMesh;
    }

}