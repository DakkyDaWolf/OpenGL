#include "pch.h"
#include "ModelManager.h"

using namespace std;

namespace Library
{
    pair<Model, vector<MeshResource>>& ModelManager::LoadModel(const std::string& fileName)
    {
        Model loadedModel(fileName, true);

        auto assembledEntry = make_pair(fileName, make_pair(move(loadedModel), vector<MeshResource>()));

        for (auto& mesh : assembledEntry.second.first.Meshes())
        {
            assembledEntry.second.second.push_back(MeshResource(*mesh));
        }

        ++ModelsLoaded;

        return RegisteredModels.emplace(move(assembledEntry)).first->second;
    }

    Model& ModelManager::GetModel(const std::string& fileName)
    {
        if (fileName.empty()) throw runtime_error("invalid filename: empty");

        if (!RegisteredModels.count(fileName)) return LoadModel(fileName).first;

        return RegisteredModels.at(fileName).first;
    }

    MeshResource& ModelManager::GetMesh(const std::string& fileName, size_t index)
    {
        if (fileName.empty()) throw runtime_error("invalid filename: empty");

        if (!RegisteredModels.count(fileName)) return LoadModel(fileName).second[index];

        return RegisteredModels.at(fileName).second[index];
    }

    std::vector<MeshResource>& ModelManager::GetMeshes(const std::string& fileName)
    {
        if (fileName.empty()) throw runtime_error("invalid filename: empty");

        if (!RegisteredModels.count(fileName)) return LoadModel(fileName).second;

        return RegisteredModels.at(fileName).second;
    }

}