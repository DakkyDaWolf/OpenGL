#include "pch.h"
#include "RenderedMesh.h"
#include "Model.h"
#include "PointLight.h"
#include "ProxyModel.h"
#include "Camera.h"
#include "Game.h"
#include "MatrixHelper.h"
#include "SpotLight.h"
#include "ProjectingLight.h"
#include <glm/gtx/transform.hpp>
#include "TextureManager.h"
#include "ModelManager.h"

using namespace glm;
using namespace std;

namespace Library
{

	RTTI_DEFINITIONS(RenderedMesh)

	RenderedMesh::RenderedMesh(Library::Game& game, std::shared_ptr<Camera> camera, const std::string& objectFilename, const std::string& textureFilename, int meshIndex) :
		DrawableGameComponent(game, move(camera)), mObjectFilename(objectFilename), mTextureFilename(textureFilename), mTextured(!textureFilename.empty()),
		MovableGameObject(game), mMeshIndex(meshIndex)
	{
	}

	RenderedMesh::~RenderedMesh()
	{
		glDeleteSamplers(1, &mTrilinearSampler);
		glDeleteTextures(1, &mColorTextureID);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void RenderedMesh::Initialize()
	{
		//load texture if specified
		if (!mTextureFilename.empty())
		{
			mColorTextureID = TextureManager::RegisterTexture(mTextureFilename);
			if (mColorTextureID == 0)
			{
				throw std::runtime_error("SOIL_load_OGL_texture() failed.");
			}

			mAlbedo.a = 0;
			mTextured = true;
		}

		if (!ProgramsInitialized)
		{
			mShaderProgram = new SpotLightEffect();
			mShaderProgramShadowed = new SpotLightShadowedEffect();
			mShaderProgramDepth = new DepthPass();
			mDeferredProgram = new DeferredEffect();

			// Create the trilinear texture sampler
			glGenSamplers(1, &mTrilinearSampler);
			glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glGenSamplers(1, &mShadowSampler);
			//glSamplerParameteri(mShadowSampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			//glSamplerParameteri(mShadowSampler, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			glSamplerParameteri(mShadowSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glSamplerParameteri(mShadowSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glSamplerParameteri(mShadowSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glSamplerParameteri(mShadowSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Build the shader programs
			{
				vector<ShaderDefinition> shaders;
				shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/SpotlightBasic.vert"));
				shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/SpotlightBasic.frag"));
				mShaderProgram->BuildProgram(shaders);
			}
			{
				vector<ShaderDefinition> shaders;
				shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/DepthPass.vert"));
				shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/DepthPass.frag"));
				mShaderProgramDepth->BuildProgram(shaders);
			}
			{
				vector<ShaderDefinition> shaders;
				shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/SpotlightShadowed.vert"));
				shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/SpotlightShadowed.frag"));
				mShaderProgramShadowed->BuildProgram(shaders);
			}
			{
				vector<ShaderDefinition> shaders;
				shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/Deferred.vert"));
				shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/Deferred.frag"));
				mDeferredProgram->BuildProgram(shaders);
			}

			ProgramsInitialized = true;
		}		

		// Load the model
		auto& loadedMesh = ModelManager::GetMesh(mObjectFilename, mMeshIndex);

		// Create the vertex and index buffers
		VertexPositionTextureNormal::CreateVertexBuffer(loadedMesh.GetMesh(), mVertexBuffer);

		mIndexBuffer = loadedMesh.IndexBuffer();
		mIndexCount = loadedMesh.IndexCount();

		// Create the vertex array objects
		glGenVertexArrays(1, &mVertexArrayObject);
		mDeferredProgram->Initialize(mVertexArrayObject);
		mShaderProgram->Initialize(mVertexArrayObject);
		mShaderProgramDepth->Initialize(mVertexArrayObject);
		mShaderProgramShadowed->Initialize(mVertexArrayObject);
		glBindVertexArray(0);

		Reset();
	}

	void RenderedMesh::Update(const Library::GameTime& gameTime)
	{
		MovableGameObject::Update(gameTime);
	}

	void RenderedMesh::Draw(const Library::GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram->Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * Transform() * mWorldMatrix;
		mShaderProgram->WorldViewProjection() << wvp;
		mShaderProgram->World() << Transform() * mWorldMatrix;

		mShaderProgram->CameraPosition() << mCamera->Position();
		mShaderProgram->SpecularColor() << mSpecularColor;
		mShaderProgram->SpecularPower() << mSpecularPower;

		mShaderProgram->FogColor() << mFogColor;
		mShaderProgram->FogStart() << mFogStart;
		mShaderProgram->FogRange() << mFogRange;

		mShaderProgram->AmbientColor() << (mAmbientLight ? mAmbientLight->Color() : vec4(vec3(mAmbientIntensity), 1));

		mShaderProgram->LightColor() << (mSpotlight ? mSpotlight->Color() : glm::vec4(0));
		mShaderProgram->LightLookDirection() << (mSpotlight ? mSpotlight->Forward() : glm::vec3(1, 0, 0));
		mShaderProgram->LightPosition() << (mSpotlight ? mSpotlight->Position() : glm::vec3(0));
		mShaderProgram->LightFalloffRange() << (mSpotlight ? mSpotlight->AttenuationRadius() : 1.f);
		mShaderProgram->LightInnerAngle() << (mSpotlight ? mSpotlight->InnerAngle() : 0.25f);
		mShaderProgram->LightOuterAngle() << (mSpotlight ? mSpotlight->OuterAngle() : 0.3f);
		//mShaderProgram.EmissiveIntensity() << mEmissiveIntensity();

		mShaderProgram->Albedo() << mAlbedo;

		glBindSampler(0, mTrilinearSampler);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mColorTextureID);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void RenderedMesh::DrawShadowed(const Library::GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgramShadowed->Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * Transform() * mWorldMatrix;
		mShaderProgramShadowed->WorldViewProjection() << wvp;
		mShaderProgramShadowed->World() << Transform() * mWorldMatrix;

		mShaderProgramShadowed->CameraPosition() << mCamera->Position();
		mShaderProgramShadowed->SpecularColor() << mSpecularColor;
		mShaderProgramShadowed->SpecularPower() << mSpecularPower;

		mShaderProgramShadowed->FogColor() << mFogColor;
		mShaderProgramShadowed->FogStart() << mFogStart;
		mShaderProgramShadowed->FogRange() << mFogRange;

		mShaderProgramShadowed->AmbientColor() << (mAmbientLight ? mAmbientLight->Color() : glm::vec4(1));

		mShaderProgramShadowed->LightColor() << (mSpotlight ? mSpotlight->Color() : glm::vec4(0));
		mShaderProgramShadowed->LightLookDirection() << (mSpotlight ? -mSpotlight->Forward() : glm::vec3(1, 0, 0));
		mShaderProgramShadowed->LightPosition() << (mSpotlight ? mSpotlight->Position() : glm::vec3(0));
		mShaderProgramShadowed->LightFalloffRange() << (mSpotlight ? mSpotlight->AttenuationRadius() : 1.f);
		mShaderProgramShadowed->LightInnerAngle() << (mSpotlight ? mSpotlight->InnerAngle() : 0.25f);
		mShaderProgramShadowed->LightOuterAngle() << (mSpotlight ? mSpotlight->OuterAngle() : 0.3f);
		mShaderProgramShadowed->LightWVP() << (mSpotlight ? mSpotlight->ViewProjectionMatrix() : mat4(1));
		mShaderProgramShadowed->ShadowDepthTolerance() << mShadowDepthBias;

		mShaderProgramShadowed->Albedo() << mAlbedo;

		glBindSampler(0, mTrilinearSampler);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mColorTextureID);

		glBindSampler(1, mTrilinearSampler);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, (mSpotlight ? mSpotlight->DepthMapTexture() : 0));

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void RenderedMesh::DrawToGBuffer(const Library::GameTime& gameTime)
	{
		if (!mDeferredBuffer) return Draw(gameTime);

		glBindFramebuffer(GL_FRAMEBUFFER, mDeferredBuffer->BufferID());
		
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mDeferredProgram->Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * Transform() * mWorldMatrix;
		mDeferredProgram->WorldViewProjection() << wvp;
		mDeferredProgram->World() << Transform() * mWorldMatrix;

		mDeferredProgram->SpecularPower() << mSpecularPower;

		glBindSampler(0, mTrilinearSampler);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mColorTextureID);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderedMesh::DepthTest(ProjectingLight& lightSource)
	{
		mShaderProgramDepth->Use();

		glViewport(0, 0, lightSource.DepthMapWidth(), lightSource.DepthMapHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, lightSource.FrameBuffer());

		mat4 wvp = lightSource.ViewProjectionMatrix() * Transform() * mWorldMatrix;
		mShaderProgramDepth->WorldViewProjection() << wvp;

		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, DrawableGameComponent::mGame->ScreenWidth(), DrawableGameComponent::mGame->ScreenHeight());
	}

	void RenderedMesh::SetGBuffer(DeferredFramebuffer& gbuffer)
	{
		mDeferredBuffer = &gbuffer;
	}

	void RenderedMesh::SetAlbedo(glm::vec4 newColor)
	{
		mAlbedo = newColor;
	}

	void RenderedMesh::SetDepthBias(float bias)
	{
		mShadowDepthBias = bias;
	}

	void RenderedMesh::SetAmbientLight(std::shared_ptr<Light> newLight)
	{
		mAmbientLight = newLight;
	}

	void RenderedMesh::SetSpotLight(std::shared_ptr<ProjectingLight> newLight)
	{
		mSpotlight = newLight;
	}
}