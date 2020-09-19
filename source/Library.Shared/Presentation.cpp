#include "pch.h"
#include "Presentation.h"
#include "VertexDeclarations.h"
#include "ShaderProgram.h"
#include "Game.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Presentation)

		Presentation::Presentation(Game& game) :
		DrawableGameComponent(game)
	{
	}

	void Presentation::Initialize()
	{
		DrawableGameComponent::Initialize();

		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/FullscreenTexture.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/FullscreenTexture.frag"));
		mShaderProgram.BuildProgram(shaders);


		// Create the trilinear texture sampler
		glGenSamplers(1, &mTextureSampler);
		glSamplerParameteri(mTextureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(mTextureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(mTextureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(mTextureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindVertexArray(0);

		// Create the normal mapping vertex array object
		glGenVertexArrays(1, &mRectVAO);
		glBindVertexArray(mRectVAO);

		// Create the normal mapping vertex buffer
		const VertexPositionTexture rectVertices[] =
		{
			VertexPositionTexture(vec4(-1.f, -1.f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
			VertexPositionTexture(vec4(-1.f, 1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
			VertexPositionTexture(vec4(1.0f, 1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
			VertexPositionTexture(vec4(1.0f, -1.f, 0.0f, 1.0f), vec2(1.0f, 1.0f))
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

		mIndexCount = GLuint(std::size(indices));

		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndexCount, indices, GL_STATIC_DRAW);
		
	}

	void Presentation::Draw(const GameTime& /*gameTime*/)
	{
		if (mSelectedSlide < mSlides.size())
		{
			glDisable(GL_DEPTH_TEST);
			glViewport(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight());
			glBindVertexArray(mRectVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mRectVertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

			mShaderProgram.Use();

			glBindSampler(0, mTextureSampler);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mSlides[mSelectedSlide]->TextureID());

			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glEnable(GL_DEPTH_TEST);
		}
	}

	void Presentation::AddSlide(const std::string& filename)
	{
		mSlides.push_back(make_shared<Texture>(filename));
	}

	void Presentation::AddSlide(std::shared_ptr<Texture> newSlide)
	{
		mSlides.push_back(newSlide);
	}

	void Presentation::Next()
	{
		if (mSelectedSlide < mSlides.size()) ++mSelectedSlide;
	}

	void Presentation::Previous()
	{
		if (mSelectedSlide > 0) --mSelectedSlide;
	}

}