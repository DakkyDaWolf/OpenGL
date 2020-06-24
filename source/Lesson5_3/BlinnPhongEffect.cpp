#include "pch.h"
#include "BlinnPhongEffect.h"
#include "VertexDeclarations.h"
#include "Mesh.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
    RTTI_DEFINITIONS(BlinnPhongEffect)	

    BlinnPhongEffect::BlinnPhongEffect() :
		SHADER_VARIABLE_INITIALIZATION(WorldViewProjection), SHADER_VARIABLE_INITIALIZATION(World),
		SHADER_VARIABLE_INITIALIZATION(AmbientColor), SHADER_VARIABLE_INITIALIZATION(LightColor),
		SHADER_VARIABLE_INITIALIZATION(LightDirection), SHADER_VARIABLE_INITIALIZATION(CameraPosition),
		SHADER_VARIABLE_INITIALIZATION(SpecularColor), SHADER_VARIABLE_INITIALIZATION(SpecularPower)
    {
    }

    SHADER_VARIABLE_DEFINITION(BlinnPhongEffect, WorldViewProjection)
	SHADER_VARIABLE_DEFINITION(BlinnPhongEffect, World)
	SHADER_VARIABLE_DEFINITION(BlinnPhongEffect, AmbientColor)
	SHADER_VARIABLE_DEFINITION(BlinnPhongEffect, LightColor)
	SHADER_VARIABLE_DEFINITION(BlinnPhongEffect, LightDirection)
	SHADER_VARIABLE_DEFINITION(BlinnPhongEffect, CameraPosition)
	SHADER_VARIABLE_DEFINITION(BlinnPhongEffect, SpecularColor)
	SHADER_VARIABLE_DEFINITION(BlinnPhongEffect, SpecularPower)

	void BlinnPhongEffect::Initialize(GLuint vertexArrayObject)
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

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));
	}
}