#include "pch.h"
#include "ScreenRect.h"
#include "Game.h"
#include "ShaderProgram.h"
#include <vector>
#include "VertexDeclarations.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(ScreenRect)

	ScreenRect::ScreenRect(Game& game, const std::string& filename) :
		DrawableGameComponent(game),
		Rectangle(Point(0,0), Point(game.ScreenWidth(), game.ScreenHeight())),
		mTextureFilename(filename)
	{
	}

	void ScreenRect::Initialize()
	{
		if (!mTextureFilename.empty())
		{
			mTextureID = SOIL_load_OGL_texture(mTextureFilename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
			if (mTextureID == 0)
			{
				throw std::runtime_error("SOIL_load_OGL_texture() failed.");
			}
		}

		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/2DTexture.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/2DTexture.frag"));
		mShaderProgram.BuildProgram(shaders);


		// Create the trilinear texture sampler
		glGenSamplers(1, &mTrilinearSampler);
		glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindVertexArray(0);

		// Create the normal mapping vertex array object
		glGenVertexArrays(1, &mRectVAO);
		glBindVertexArray(mRectVAO);

		// Create the normal mapping vertex buffer
		const VertexPositionTexture rectVertices[] =
		{
			VertexPositionTexture(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
			VertexPositionTexture(vec4(0.0f, 1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
			VertexPositionTexture(vec4(1.0f, 1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
			VertexPositionTexture(vec4(1.0f, 0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f))
		};

		VertexPositionTexture::CreateVertexBuffer(rectVertices, mRectVertexBuffer);
		mShaderProgram.Initialize(mRectVAO);

		glBindVertexArray(0);

		// Create the index buffer
		const uint32_t indices[] =
		{
			0, 2, 1,
			0, 3, 2
		};

		mIndexCount = std::size(indices);

		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)* mIndexCount, indices, GL_STATIC_DRAW);
	}

	void ScreenRect::Draw(const GameTime& /*gameTime*/)
	{
		glViewport(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight());
		glBindVertexArray(mRectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mRectVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram.Use();

		mShaderProgram.ScreenspaceProjection() << ScreenspaceTransform();

		glBindSampler(0, mTrilinearSampler);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void ScreenRect::SetTexture(GLuint textureID)
	{
		mTextureID = textureID;
	}

	void ScreenRect::SetPosition(const glm::vec2& position)
	{
		mPosition = position;
		mScreenspaceTransformDirty = true;
	}

	void ScreenRect::SetDimensions(const glm::vec2& dimensions)
	{
		mDimensions = dimensions;
		mScreenspaceTransformDirty = true;
	}

	const glm::mat4& ScreenRect::ScreenspaceTransform()
	{
		if (mScreenspaceTransformDirty)
		{
			int w = mGame->ScreenWidth();
			int h = mGame->ScreenHeight();
			
			mScreenspaceTransform = mat4(1);
			mScreenspaceTransform[0][0] = 2.f * mDimensions.x / w;
			mScreenspaceTransform[1][1] = 2.f * mDimensions.y / h;
			mScreenspaceTransform[2][2] = 0.f;
			mScreenspaceTransform[3] = vec4((2.f * mPosition.x / w) - 1.f, (2 * mPosition.y / h) - 1.f, 0, 1);
			mScreenspaceTransformDirty = false;
		}
		return mScreenspaceTransform;
	}

}