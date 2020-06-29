#include "pch.h"
#include "SpotLightEffectAlbedo.h"
#include "Mesh.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(SpotLightEffectAlbedo)

	SpotLightEffectAlbedo::SpotLightEffectAlbedo() :
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection), SHADER_VARIABLE_INITIALIZATION(World),
		SHADER_VARIABLE_INITIALIZATION(AmbientColor), SHADER_VARIABLE_INITIALIZATION(LightColor),
		SHADER_VARIABLE_INITIALIZATION(LightLookDirection), SHADER_VARIABLE_INITIALIZATION(CameraPosition),
		SHADER_VARIABLE_INITIALIZATION(SpecularColor), SHADER_VARIABLE_INITIALIZATION(SpecularPower),
		SHADER_VARIABLE_INITIALIZATION(FogColor), SHADER_VARIABLE_INITIALIZATION(FogStart),
		SHADER_VARIABLE_INITIALIZATION(FogRange),
		SHADER_VARIABLE_INITIALIZATION(LightPosition), SHADER_VARIABLE_INITIALIZATION(LightInnerAngle),
		SHADER_VARIABLE_INITIALIZATION(LightOuterAngle), SHADER_VARIABLE_INITIALIZATION(LightFalloffRange),
		SHADER_VARIABLE_INITIALIZATION(Albedo)
	{
	}

	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, WorldViewProjection)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, World)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, AmbientColor)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, LightColor)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, LightLookDirection)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, CameraPosition)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, SpecularColor)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, SpecularPower)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, FogColor)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, FogStart)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, FogRange)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, LightPosition)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, LightInnerAngle)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, LightOuterAngle)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, LightFalloffRange)
	SHADER_VARIABLE_DEFINITION(SpotLightEffectAlbedo, Albedo)


	void SpotLightEffectAlbedo::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(WorldViewProjection)
		SHADER_VARIABLE_INSTANTIATE(World)
		SHADER_VARIABLE_INSTANTIATE(AmbientColor)
		SHADER_VARIABLE_INSTANTIATE(LightColor)
		SHADER_VARIABLE_INSTANTIATE(LightLookDirection)
		SHADER_VARIABLE_INSTANTIATE(CameraPosition)
		SHADER_VARIABLE_INSTANTIATE(SpecularColor)
		SHADER_VARIABLE_INSTANTIATE(SpecularPower)
		SHADER_VARIABLE_INSTANTIATE(FogColor)
		SHADER_VARIABLE_INSTANTIATE(FogStart)
		SHADER_VARIABLE_INSTANTIATE(FogRange)
		SHADER_VARIABLE_INSTANTIATE(LightPosition)
		SHADER_VARIABLE_INSTANTIATE(LightInnerAngle)
		SHADER_VARIABLE_INSTANTIATE(LightOuterAngle)
		SHADER_VARIABLE_INSTANTIATE(LightFalloffRange)
		SHADER_VARIABLE_INSTANTIATE(Albedo)

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));
	}
}