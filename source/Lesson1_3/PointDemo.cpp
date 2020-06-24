#include "pch.h"
#include "PointDemo.h"
#include "Utility.h"

using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(PointDemo)

	PointDemo::PointDemo(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	PointDemo::~PointDemo()
	{
		glDeleteProgram(mShaderProgram);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void PointDemo::Initialize()
	{
		// Load and compile vertex shader
		vector<char> vertexShaderSource;
		Utility::LoadBinaryFile("Content/Effects/PointDemo.vert", vertexShaderSource);
		GLchar* sourcePointer = &vertexShaderSource[0];
		GLint length = static_cast<GLint>(vertexShaderSource.size());

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &sourcePointer, &length);
		glCompileShader(vertexShader);

		// Load and compile fragment shader
		vector<char> fragmentShaderSource;
		Utility::LoadBinaryFile("Content/Effects/PointDemo.frag", fragmentShaderSource);
		sourcePointer = &fragmentShaderSource[0];
		length = static_cast<GLint>(fragmentShaderSource.size());

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &sourcePointer, &length);
		glCompileShader(fragmentShader);

		// Create and link the shader program
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, vertexShader);
		glAttachShader(mShaderProgram, fragmentShader);
		glLinkProgram(mShaderProgram);

		// Clean up temporary shader objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Create vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
	}

	void PointDemo::Draw(const GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glUseProgram(mShaderProgram);
		glPointSize(80.0f);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}
}