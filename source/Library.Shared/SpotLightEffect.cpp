#include "pch.h"
#include "SpotLightEffect.h"
#include "Mesh.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(SpotLightEffect)

	SpotLightEffect::SpotLightEffect() :
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

	SHADER_VARIABLE_DEFINITION(SpotLightEffect, WorldViewProjection)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, World)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, AmbientColor)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, LightColor)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, LightLookDirection)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, CameraPosition)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, SpecularColor)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, SpecularPower)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, FogColor)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, FogStart)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, FogRange)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, LightPosition)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, LightInnerAngle)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, LightOuterAngle)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, LightFalloffRange)
	SHADER_VARIABLE_DEFINITION(SpotLightEffect, Albedo)


	void SpotLightEffect::Initialize(GLuint vertexArrayObject)
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

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));
	}
}