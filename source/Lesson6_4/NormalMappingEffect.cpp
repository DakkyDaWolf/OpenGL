#include "pch.h"
#include "NormalMappingEffect.h"
#include "Mesh.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
    RTTI_DEFINITIONS(NormalMappingEffect)	

    NormalMappingEffect::NormalMappingEffect() :
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection), SHADER_VARIABLE_INITIALIZATION(World),
		SHADER_VARIABLE_INITIALIZATION(AmbientColor), SHADER_VARIABLE_INITIALIZATION(LightColor),
		SHADER_VARIABLE_INITIALIZATION(LightDirection), SHADER_VARIABLE_INITIALIZATION(CameraPosition),
		SHADER_VARIABLE_INITIALIZATION(SpecularColor), SHADER_VARIABLE_INITIALIZATION(SpecularPower),
		SHADER_VARIABLE_INITIALIZATION(FogColor), SHADER_VARIABLE_INITIALIZATION(FogStart),
		SHADER_VARIABLE_INITIALIZATION(FogRange)
    {
    }

    SHADER_VARIABLE_DEFINITION(NormalMappingEffect, WorldViewProjection)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, World)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, AmbientColor)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, LightColor)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, LightDirection)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, CameraPosition)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, SpecularColor)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, SpecularPower)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, FogColor)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, FogStart)
	SHADER_VARIABLE_DEFINITION(NormalMappingEffect, FogRange)

	void NormalMappingEffect::Initialize(GLuint vertexArrayObject)
    {
        ShaderProgram::Initialize(vertexArrayObject);

        SHADER_VARIABLE_INSTANTIATE(WorldViewProjection)
		SHADER_VARIABLE_INSTANTIATE(World)
		SHADER_VARIABLE_INSTANTIATE(AmbientColor)
		SHADER_VARIABLE_INSTANTIATE(LightColor)
		SHADER_VARIABLE_INSTANTIATE(LightDirection)
		SHADER_VARIABLE_INSTANTIATE(CameraPosition)
		SHADER_VARIABLE_INSTANTIATE(SpecularColor)
		SHADER_VARIABLE_INSTANTIATE(SpecularPower)
		SHADER_VARIABLE_INSTANTIATE(FogColor)
		SHADER_VARIABLE_INSTANTIATE(FogStart)
		SHADER_VARIABLE_INSTANTIATE(FogRange)

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormalTangentBinormal), (void*)offsetof(VertexPositionTextureNormalTangentBinormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormalTangentBinormal), (void*)offsetof(VertexPositionTextureNormalTangentBinormal, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormalTangentBinormal), (void*)offsetof(VertexPositionTextureNormalTangentBinormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Tangent), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormalTangentBinormal), (void*)offsetof(VertexPositionTextureNormalTangentBinormal, Tangent));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Tangent));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Binormal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormalTangentBinormal), (void*)offsetof(VertexPositionTextureNormalTangentBinormal, Binormal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Binormal));
    }
}