#include "pch.h"
#include "EnvironmentMappingEffect.h"
#include "Mesh.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
    RTTI_DEFINITIONS(EnvironmentMappingEffect)	

    EnvironmentMappingEffect::EnvironmentMappingEffect() :
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection), SHADER_VARIABLE_INITIALIZATION(World),
		SHADER_VARIABLE_INITIALIZATION(AmbientColor), SHADER_VARIABLE_INITIALIZATION(EnvironmentColor),
		SHADER_VARIABLE_INITIALIZATION(ReflectionAmount), SHADER_VARIABLE_INITIALIZATION(CameraPosition)
    {
    }

    SHADER_VARIABLE_DEFINITION(EnvironmentMappingEffect, WorldViewProjection)
	SHADER_VARIABLE_DEFINITION(EnvironmentMappingEffect, World)
	SHADER_VARIABLE_DEFINITION(EnvironmentMappingEffect, AmbientColor)
	SHADER_VARIABLE_DEFINITION(EnvironmentMappingEffect, EnvironmentColor)
	SHADER_VARIABLE_DEFINITION(EnvironmentMappingEffect, ReflectionAmount)
	SHADER_VARIABLE_DEFINITION(EnvironmentMappingEffect, CameraPosition)

	void EnvironmentMappingEffect::Initialize(GLuint vertexArrayObject)
    {
        ShaderProgram::Initialize(vertexArrayObject);

        SHADER_VARIABLE_INSTANTIATE(WorldViewProjection)
		SHADER_VARIABLE_INSTANTIATE(World)
		SHADER_VARIABLE_INSTANTIATE(AmbientColor)
		SHADER_VARIABLE_INSTANTIATE(EnvironmentColor)
		SHADER_VARIABLE_INSTANTIATE(ReflectionAmount)
		SHADER_VARIABLE_INSTANTIATE(CameraPosition)

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));
    }
}