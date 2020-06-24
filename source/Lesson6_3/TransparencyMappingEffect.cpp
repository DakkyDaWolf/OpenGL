#include "pch.h"
#include "TransparencyMappingDemo.h"
#include "Mesh.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(TransparencyMappingEffect)

	TransparencyMappingEffect::TransparencyMappingEffect() :
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection), SHADER_VARIABLE_INITIALIZATION(World),
		SHADER_VARIABLE_INITIALIZATION(AmbientColor), SHADER_VARIABLE_INITIALIZATION(LightColor),
		SHADER_VARIABLE_INITIALIZATION(LightDirection), SHADER_VARIABLE_INITIALIZATION(CameraPosition),
		SHADER_VARIABLE_INITIALIZATION(SpecularColor), SHADER_VARIABLE_INITIALIZATION(SpecularPower),
		SHADER_VARIABLE_INITIALIZATION(FogColor), SHADER_VARIABLE_INITIALIZATION(FogStart),
		SHADER_VARIABLE_INITIALIZATION(FogRange)
	{
	}

	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, WorldViewProjection)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, World)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, AmbientColor)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, LightColor)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, LightDirection)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, CameraPosition)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, SpecularColor)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, SpecularPower)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, FogColor)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, FogStart)
	SHADER_VARIABLE_DEFINITION(TransparencyMappingEffect, FogRange)

	void TransparencyMappingEffect::Initialize(GLuint vertexArrayObject)
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

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));
	}
}