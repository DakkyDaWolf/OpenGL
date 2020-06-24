#include "pch.h"
#include "ColoredTriangleDemo.h"
#include "ShaderProgram.h"
#include "ColorHelper.h"
#include "Utility.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(ColoredTriangleDemo)

	ColoredTriangleDemo::ColoredTriangleDemo(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	ColoredTriangleDemo::~ColoredTriangleDemo()
	{
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void ColoredTriangleDemo::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/ColoredTriangleDemo.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/ColoredTriangleDemo.frag"));
		mShaderProgram.BuildProgram(shaders);
		
		// Create the vertex buffer
		const VertexPositionColor vertices[] =
		{
			VertexPositionColor(vec4(-0.5f, -0.5f, 0.5f, 1.0f), ColorHelper::Cyan),
			VertexPositionColor(vec4(0.0f, 0.5f, 0.5f, 1.0f), ColorHelper::Purple),
			VertexPositionColor(vec4(0.5f, -0.5f, 0.5f, 1.0f), ColorHelper::Yellow)
		};

		glGenBuffers(1, &mVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Create vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
		glBindVertexArray(mVertexArrayObject);

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Color));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Color));

		glBindVertexArray(0);
	}

	void ColoredTriangleDemo::Draw(const GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glUseProgram(mShaderProgram.Program());

		const GLsizei vertexCount = 3;
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glBindVertexArray(0);
	}
}