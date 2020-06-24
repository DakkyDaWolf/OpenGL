#include "pch.h"
#include "PointLightEffect.h"
#include "VertexDeclarations.h"
#include "Mesh.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
    RTTI_DEFINITIONS(PointLightEffect)	

    PointLightEffect::PointLightEffect() : 
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection), SHADER_VARIABLE_INITIALIZATION(World),
		SHADER_VARIABLE_INITIALIZATION(AmbientColor), SHADER_VARIABLE_INITIALIZATION(LightColor),
		SHADER_VARIABLE_INITIALIZATION(LightPosition), SHADER_VARIABLE_INITIALIZATION(LightRadius),
		SHADER_VARIABLE_INITIALIZATION(CameraPosition), SHADER_VARIABLE_INITIALIZATION(SpecularColor),
		SHADER_VARIABLE_INITIALIZATION(SpecularPower), SHADER_VARIABLE_INITIALIZATION(EmissiveIntensity)
    {
    }

    SHADER_VARIABLE_DEFINITION(PointLightEffect, WorldViewProjection)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, World)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, AmbientColor)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, LightColor)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, LightPosition)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, LightRadius)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, CameraPosition)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, SpecularColor)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, SpecularPower)
	SHADER_VARIABLE_DEFINITION(PointLightEffect, EmissiveIntensity)

	void PointLightEffect::Initialize(GLuint vertexArrayObject)
    {
        ShaderProgram::Initialize(vertexArrayObject);

        SHADER_VARIABLE_INSTANTIATE(WorldViewProjection)
		SHADER_VARIABLE_INSTANTIATE(World)
		SHADER_VARIABLE_INSTANTIATE(AmbientColor)
		SHADER_VARIABLE_INSTANTIATE(LightColor)
		SHADER_VARIABLE_INSTANTIATE(LightPosition)
		SHADER_VARIABLE_INSTANTIATE(LightRadius)
		SHADER_VARIABLE_INSTANTIATE(CameraPosition)
		SHADER_VARIABLE_INSTANTIATE(SpecularColor)
		SHADER_VARIABLE_INSTANTIATE(SpecularPower)
		SHADER_VARIABLE_INSTANTIATE(EmissiveIntensity)

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));
    }
}