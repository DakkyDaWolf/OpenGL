#include "pch.h"
#include "DebugTexture.h"
#include "Camera.h"
#include "Utility.h"
#include "VertexDeclarations.h"
#include "Game.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(DebugTexture)

	DebugTexture::DebugTexture(Game& game, float scale) :
		DrawableGameComponent(game),
		mScale(std::clamp<float>(scale, 0.01f, 1.f))
	{
	}

	DebugTexture::~DebugTexture()
	{
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	const float DebugTexture::Scale() const
	{
		return mScale;
	}

	void DebugTexture::ApplyScale(float scale)
	{
		mScale = scale;
		InitializeDebugTexture();
	}

	GLuint DebugTexture::TextureID() const
	{
		return mColorTextureID;
	}

	void DebugTexture::SetTextureID(GLuint textureID)
	{
		mColorTextureID = textureID;
	}

	void DebugTexture::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/FlatTexture.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/FlatTexture.frag"));
		mShaderProgram.BuildProgram(shaders);

		InitializeDebugTexture();

		// Create the vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
		mShaderProgram.Initialize(mVertexArrayObject);
		glBindVertexArray(0);
	}

	void DebugTexture::Draw(const GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

		mShaderProgram.Use();

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glBindTexture(GL_TEXTURE_2D, mColorTextureID);
		glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
		glBindVertexArray(0);
	}

	void DebugTexture::InitializeDebugTexture()
	{
		mVertexCount = 6;
		vector<VertexPositionTexture> vertices;
		vertices.resize(mVertexCount);
		const float maxX = mScale / mGame->AspectRatio();
		const float maxY = mScale * mGame->AspectRatio();

		vertices[0] = VertexPositionTexture(vec4(0, maxY, 0, 1.f), vec2(0, 0));
		vertices[1] = VertexPositionTexture(vec4(0, 0, 0, 1.f), vec2(0, 1));
		vertices[2] = VertexPositionTexture(vec4(maxX, maxY, 0, 1.f), vec2(1, 0));

		vertices[3] = VertexPositionTexture(vec4(maxX, maxY, 0, 1.f), vec2(1, 0));
		vertices[4] = VertexPositionTexture(vec4(0, 0, 0, 1.f), vec2(0, 1));
		vertices[5] = VertexPositionTexture(vec4(maxX, 0, 0, 1.f), vec2(1, 1));

		glDeleteBuffers(1, &mVertexBuffer);

		VertexPositionTexture::CreateVertexBuffer(vertices, mVertexBuffer);
	}
}