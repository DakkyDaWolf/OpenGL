#include "pch.h"
#include "Model.h"
#include "ModelMaterial.h"

using namespace std;

namespace Library
{
    Model::Model(const string& filename, bool flipUVs)
    {
        Assimp::Importer importer;

		uint32_t flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
        if (flipUVs)
        {
            flags |= aiProcess_FlipUVs;
        }

        const aiScene* scene = importer.ReadFile(filename, flags);
        if (scene == nullptr)
        {
            throw runtime_error(importer.GetErrorString());
        }

        if (scene->HasMaterials())
        {
            for (uint32_t i = 0; i < scene->mNumMaterials; i++)
            {

                mMaterials.push_back(shared_ptr<ModelMaterial>(new ModelMaterial(*this, *scene->mMaterials[i])));
            }
        }

        if (scene->HasMeshes())
        {
            for (uint32_t i = 0; i < scene->mNumMeshes; i++)
            {	
                mMeshes.push_back(shared_ptr<Mesh>(new Mesh(*this, *(scene->mMeshes[i]))));
            }
        }
	}
	
    bool Model::HasMeshes() const
    {
        return (mMeshes.size() > 0);
    }

    bool Model::HasMaterials() const
    {
        return (mMaterials.size() > 0);
    }

    const vector<shared_ptr<Mesh>>& Model::Meshes() const
    {
        return mMeshes;
    }

    const vector<shared_ptr<ModelMaterial>>& Model::Materials() const
    {
        return mMaterials;
    }
}
