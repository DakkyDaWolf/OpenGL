#include "pch.h"
#include "DeferredAmbientLight.h"
#include "VertexDeclarations.h"
#include "Game.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(DeferredAmbientLight)

	DeferredAmbientLight::DeferredAmbientLight(Game& game, std::shared_ptr<DeferredFramebuffer>& gBuffer) :
		DrawableGameComponent(game), Light(game), mGBuffer(gBuffer)
	{
	}

	void DeferredAmbientLight::Initialize()
	{
		DrawableGameComponent::Initialize();

		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/DeferredAmbientLight.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/DeferredAmbientLight.frag"));
		mShaderProgram.BuildProgram(shaders);


		// Create the trilinear texture sampler
		glGenSamplers(1, &mTextureSampler);
		glSamplerParameteri(mTextureSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glSamplerParameteri(mTextureSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(mTextureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(mTextureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindVertexArray(0);

		// Create the normal mapping vertex array object
		glGenVertexArrays(1, &mRectVAO);
		glBindVertexArray(mRectVAO);

		// Create the normal mapping vertex buffer
		const VertexPositionTexture rectVertices[] =
		{
			VertexPositionTexture(vec4(-1.f, -1.f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
			VertexPositionTexture(vec4(-1.f, 1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
			VertexPositionTexture(vec4(1.0f, 1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
			VertexPositionTexture(vec4(1.0f, -1.f, 0.0f, 1.0f), vec2(1.0f, 0.0f))
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

	void DeferredAmbientLight::Draw(const GameTime& /*gameTime*/)
	{
		glViewport(0, 0, DrawableGameComponent::mGame->ScreenWidth(), DrawableGameComponent::mGame->ScreenHeight());
		glBindVertexArray(mRectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mRectVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram.Use();
		mShaderProgram.LightColor() << mColor * mIntensity;

		glBindSampler(0, mTextureSampler);
		glBindSampler(1, mTextureSampler);
		glBindSampler(2, mTextureSampler);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mGBuffer->PositionID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mGBuffer->NormalID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mGBuffer->ColorID());

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	float DeferredAmbientLight::Intensity() const
	{
		return mIntensity;
	}

	void DeferredAmbientLight::SetIntensity(float intensity)
	{
		mIntensity = std::clamp(intensity, 0.f, 1.f);
	}

}