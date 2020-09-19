#include "pch.h"
#include "DeferredScreen.h"
#include "VertexDeclarations.h"
#include "Game.h"

using namespace glm;
using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(DeferredScreen)

	DeferredScreen::DeferredScreen(Game& game, shared_ptr<Camera>& camera, shared_ptr<DeferredFramebuffer>& fbo, shared_ptr<ProjectingLight>& light) :
		DrawableGameComponent(game), mFramebuffer(fbo), mLight(light), mCamera(camera)
	{
	}

	void DeferredScreen::Initialize()
	{
		DrawableGameComponent::Initialize();

		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/DeferredLighting.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/DeferredLighting.frag"));
		mDeferredShader.BuildProgram(shaders);


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

		mDeferredShader.Initialize(mRectVAO);

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

	void DeferredScreen::Draw(const GameTime& /*gameTime*/)
	{
		glViewport(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight());
		glBindVertexArray(mRectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mRectVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mDeferredShader.Use();

		mDeferredShader.AmbientColor() << vec4(0); //(mAmbientLight ? mAmbientLight->Color() : ColorHelper::LightGray);

		mDeferredShader.CameraPosition() << mCamera->Position();

		mDeferredShader.LightColor() << mLight->Color();
		mDeferredShader.LightFalloffRange() << mLight->AttenuationRadius();
		mDeferredShader.LightInnerAngle() << mLight->InnerAngle();
		mDeferredShader.LightOuterAngle() << mLight->OuterAngle();
		mDeferredShader.LightLookDirection() << -mLight->Forward();
		mDeferredShader.LightPosition() << mLight->Position();

		mDeferredShader.FogColor() << mFogColor;
		mDeferredShader.FogStart() << mFogStart;
		mDeferredShader.FogRange() << mFogRange;

		glBindSampler(0, mTextureSampler);
		glBindSampler(1, mTextureSampler);
		glBindSampler(2, mTextureSampler);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mFramebuffer->PositionID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mFramebuffer->NormalID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mFramebuffer->ColorID());

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void DeferredScreen::SetAmbientLight(std::shared_ptr<Light>& light)
	{
		mAmbientLight = light;
	}

}