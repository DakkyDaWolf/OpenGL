#include "pch.h"
#include "TextureRectEffect.h"
#include "Mesh.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(TextureRectEffect)

	TextureRectEffect::TextureRectEffect() :
		SHADER_VARIABLE_INITIALIZATION(ScreenspaceProjection)
	{
	}

	SHADER_VARIABLE_DEFINITION(TextureRectEffect, ScreenspaceProjection)

	void TextureRectEffect::Initialize(GLuint vertexArrayObject)
	{
		ShaderProgram::Initialize(vertexArrayObject);

		SHADER_VARIABLE_INSTANTIATE(ScreenspaceProjection)

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));
	}
}