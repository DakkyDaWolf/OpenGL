#include "pch.h"
#include "DeferredLightingEffect.h"
#include "Mesh.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(DeferredLightingEffect)

		DeferredLightingEffect::DeferredLightingEffect() :
		SHADER_VARIABLE_INITIALIZATION(ScreenspaceProjection), SHADER_VARIABLE_INITIALIZATION(AmbientColor),
		SHADER_VARIABLE_INITIALIZATION(LightPosition), SHADER_VARIABLE_INITIALIZATION(LightLookDirection),
		SHADER_VARIABLE_INITIALIZATION(LightFalloffRange), SHADER_VARIABLE_INITIALIZATION(CameraPosition),
		SHADER_VARIABLE_INITIALIZATION(FogColor), SHADER_VARIABLE_INITIALIZATION(FogStart),
		SHADER_VARIABLE_INITIALIZATION(FogRange), SHADER_VARIABLE_INITIALIZATION(LightColor),
		SHADER_VARIABLE_INITIALIZATION(LightInnerAngle), SHADER_VARIABLE_INITIALIZATION(LightOuterAngle)
	{
	}

	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, ScreenspaceProjection)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, AmbientColor)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, LightPosition)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, LightLookDirection)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, LightColor)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, LightFalloffRange)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, LightInnerAngle)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, LightOuterAngle)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, CameraPosition)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, FogColor)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, FogStart)
	SHADER_VARIABLE_DEFINITION(DeferredLightingEffect, FogRange)

		void DeferredLightingEffect::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(ScreenspaceProjection)
		SHADER_VARIABLE_INSTANTIATE(AmbientColor)
		SHADER_VARIABLE_INSTANTIATE(LightPosition)
		SHADER_VARIABLE_INSTANTIATE(LightColor)
		SHADER_VARIABLE_INSTANTIATE(LightLookDirection)
		SHADER_VARIABLE_INSTANTIATE(LightFalloffRange)
		SHADER_VARIABLE_INSTANTIATE(LightInnerAngle)
		SHADER_VARIABLE_INSTANTIATE(LightOuterAngle)
		SHADER_VARIABLE_INSTANTIATE(CameraPosition)
		SHADER_VARIABLE_INSTANTIATE(FogColor)
		SHADER_VARIABLE_INSTANTIATE(FogStart)
		SHADER_VARIABLE_INSTANTIATE(FogRange)

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));
	}
}