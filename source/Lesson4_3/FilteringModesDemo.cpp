#include "pch.h"
#include "FilteringModesDemo.h"
#include "VertexDeclarations.h"
#include "Camera.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(FilteringModesDemo)

	FilteringModesDemo::FilteringModesDemo(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	FilteringModesDemo::~FilteringModesDemo()
	{
		mGame->RemoveKeyboardHandler(mKeyboardHandler);
		glDeleteSamplers(static_cast<GLsizei>(mTextureSamplers.size()), &mTextureSamplers[0]);
		glDeleteTextures(1, &mColorTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void FilteringModesDemo::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/FilteringModesDemo.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/FilteringModesDemo.frag"));
		mShaderProgram.BuildProgram(shaders);

		const float size = 10.0f;
		const float halfSize = size / 2.0f;

		// Create the vertex buffer
		const VertexPositionTexture vertices[] =
		{
			VertexPositionTexture(vec4(-halfSize, 1.0f, 0.0, 1.0f), vec2(0.0f, 1.0f)),
			VertexPositionTexture(vec4(-halfSize, size + 1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
			VertexPositionTexture(vec4(halfSize, size + 1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
			VertexPositionTexture(vec4(halfSize, 1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f))
		};

		VertexPositionTexture::CreateVertexBuffer(vertices, mVertexBuffer);

		// Create the index buffer
		const uint32_t indices[] =
		{
			0, 2, 1,
			0, 3, 2
		};

		mIndexCount = std::size(indices);
		CreateIndexBuffer(indices, static_cast<GLsizei>(mIndexCount), mIndexBuffer);

		// Find the WVP uniform location
		mWorldViewProjectionLocation = glGetUniformLocation(mShaderProgram.Program(), "WorldViewProjection");
		if (mWorldViewProjectionLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform location.");
		}

		// Load the texture
		mColorTexture = SOIL_load_OGL_texture("Content/Textures/EarthComposite.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB);
		if (mColorTexture == 0)
		{
			throw runtime_error("SOIL_load_OGL_texture() failed.");
		}

		// Configure the texture samplers
		mTextureSamplers.resize(static_cast<size_t>(FilteringMode::End));
		glGenSamplers(static_cast<GLsizei>(mTextureSamplers.size()), &mTextureSamplers[0]);

		for (FilteringMode mode = FilteringMode(0); mode < FilteringMode::End; mode = FilteringMode(static_cast<int>(mode) + 1))
		{
			mTextureSamplersByFilteringMode[mode] = mTextureSamplers[static_cast<int>(mode)];
		}

		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::Point], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::Point], GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::Linear], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::Linear], GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::PointMipMapPoint], GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::PointMipMapPoint], GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::LinearMipMapPoint], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::LinearMipMapPoint], GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::PointMipMapLinear], GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::PointMipMapLinear], GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::TriLinear], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(mTextureSamplersByFilteringMode[FilteringMode::TriLinear], GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Create the vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
		glBindVertexArray(mVertexArrayObject);

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glBindVertexArray(0);

		// Attach the keyboard handler
		using namespace std::placeholders;
		mKeyboardHandler = bind(&FilteringModesDemo::OnKey, this, _1, _2, _3, _4);
		mGame->AddKeyboardHandler(mKeyboardHandler);
	}

	void FilteringModesDemo::Draw(const GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBindTexture(GL_TEXTURE_2D, mColorTexture);

		glBindSampler(0, mTextureSamplersByFilteringMode[mActiveFilteringMode]);

		glUseProgram(mShaderProgram.Program());

		mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
		glUniformMatrix4fv(mWorldViewProjectionLocation, 1, GL_FALSE, &wvp[0][0]);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void FilteringModesDemo::CreateIndexBuffer(const uint32_t* indices, uint32_t indexCount, GLuint& indexBuffer)
	{
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCount, indices, GL_STATIC_DRAW);
	}

	void FilteringModesDemo::OnKey(int key, int /*scancode*/, int action, int /*mods*/)
	{
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			FilteringMode activeMode = FilteringMode(static_cast<int>(mActiveFilteringMode) + 1);
			if (activeMode >= FilteringMode::End)
			{
				activeMode = FilteringMode(0);
			}

			mActiveFilteringMode = activeMode;
		}
	}
}