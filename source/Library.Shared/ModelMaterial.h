#pragma once

struct aiMaterial;
#include <vector>
#include <map>

namespace Library
{
	class Model;

	enum class TextureType
	{
		Difffuse = 0,
		SpecularMap,
		Ambient,
		Emissive,
		Heightmap,
		NormalMap,
		SpecularPowerMap,
		DisplacementMap,
		LightMap,

		Begin = Difffuse,
		End = LightMap
    };

    class ModelMaterial final
    {
        friend class Model;

    public:
        explicit ModelMaterial(Model& model);
		ModelMaterial(const ModelMaterial&) = default;
		ModelMaterial& operator=(const ModelMaterial&) = default;
		ModelMaterial(ModelMaterial&&) = default;
		ModelMaterial& operator=(ModelMaterial&&) = default;
        ~ModelMaterial() = default;

        Model& GetModel();
        const std::string& Name() const;
        const std::map<TextureType, std::vector<std::string>>& Textures() const;

    private:		
        const static std::map<TextureType, std::uint32_t> sTextureTypeMappings;

        ModelMaterial(Model& model, aiMaterial& material);        

        Model* mModel;
        std::string mName;
        std::map<TextureType, std::vector<std::string>> mTextures;	
    };
}