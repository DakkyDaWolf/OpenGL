#include "pch.h"
#include "DeferredPointLightEffect.h"
#include "Mesh.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(DeferredPointLightEffect)

	DeferredPointLightEffect::DeferredPointLightEffect() :
		SHADER_VARIABLE_INITIALIZATION(LightPosition), SHADER_VARIABLE_INITIALIZATION(CameraPosition),
		SHADER_VARIABLE_INITIALIZATION(LightColor), SHADER_VARIABLE_INITIALIZATION(LightRadius),
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection), SHADER_VARIABLE_INITIALIZATION(ScreenDimensions)
	{
	}

		SHADER_VARIABLE_DEFINITION(DeferredPointLightEffect, LightPosition)
		SHADER_VARIABLE_DEFINITION(DeferredPointLightEffect, LightColor)
		SHADER_VARIABLE_DEFINITION(DeferredPointLightEffect, LightRadius)
		SHADER_VARIABLE_DEFINITION(DeferredPointLightEffect, CameraPosition)
		SHADER_VARIABLE_DEFINITION(DeferredPointLightEffect, WorldViewProjection)
		SHADER_VARIABLE_DEFINITION(DeferredPointLightEffect, ScreenDimensions)

	void DeferredPointLightEffect::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(LightPosition)
		SHADER_VARIABLE_INSTANTIATE(LightColor)
		SHADER_VARIABLE_INSTANTIATE(LightRadius)
		SHADER_VARIABLE_INSTANTIATE(CameraPosition)
		SHADER_VARIABLE_INSTANTIATE(WorldViewProjection)
		SHADER_VARIABLE_INSTANTIATE(ScreenDimensions)

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPosition), (void*)offsetof(VertexPosition, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));
	}
}