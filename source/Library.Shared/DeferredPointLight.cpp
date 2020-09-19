#include "pch.h"
#include "DeferredPointLight.h"
#include "VertexDeclarations.h"
#include "Model.h"
#include "Camera.h"
#include "Game.h"
#include "ModelManager.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(DeferredPointLight)

		DeferredPointLight::DeferredPointLight(Game& game, std::shared_ptr<Camera>& camera) :
		DrawableGameComponent(game)
	{
		mCamera = camera;
	}

	void DeferredPointLight::Initialize()
	{
		DrawableGameComponent::Initialize();

		if (!Initialized)
		{

			mShaderProgram = new DeferredPointLightEffect();

			vector<ShaderDefinition> shaders;
			shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/DeferredPointLight.vert"));
			shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/DeferredPointLight.frag"));
			mShaderProgram->BuildProgram(shaders);


			// Create the trilinear texture sampler
			glGenSamplers(1, &mTextureSampler);
			glSamplerParameteri(mTextureSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glSamplerParameteri(mTextureSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glSamplerParameteri(mTextureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glSamplerParameteri(mTextureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindVertexArray(0);

			// Load the model
			//Model model("Content/Models/unitsphere.obj", true);
			auto& loadedMesh = ModelManager::GetMesh("Content/Models/unitsphere.obj");

			// Create the vertex and index buffers
			auto& mesh = loadedMesh.GetMesh();
			VertexPosition::CreateVertexBuffer(mesh, mVertexBuffer);
			mIndexBuffer = loadedMesh.IndexBuffer();
			mIndexCount = loadedMesh.IndexCount();

			// Create the vertex array object
			glGenVertexArrays(1, &mVAO);
			mShaderProgram->Initialize(mVAO);
			glBindVertexArray(0);

			Initialized = true;
		}
	}

	void DeferredPointLight::Update(const GameTime& /*gameTime*/)
	{
	}

	void DeferredPointLight::Draw(const GameTime& /*gameTime*/)
	{
		
		glViewport(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight());
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		if (glm::distance(mCamera->Position(), Position()) > Radius() * radiusMult)
		{
			glCullFace(GL_BACK);
		}
		else
		{
			glCullFace(GL_FRONT);
		}

		mShaderProgram->Use();

		mShaderProgram->WorldViewProjection() << mCamera->ViewProjectionMatrix() * Transform() * mWorldMatrix;

		mShaderProgram->LightColor() << mColor * mIntensity;
		mShaderProgram->LightPosition() << mPosition;
		mShaderProgram->LightRadius() << mRadius;

		mShaderProgram->CameraPosition() << mCamera->Position();

		mShaderProgram->ScreenDimensions() << vec2(mGame->ScreenWidth(), mGame->ScreenHeight());

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

	void DeferredPointLight::SetColor(glm::vec4 color)
	{
		mColor = color;
	}

	void DeferredPointLight::SetGBuffer(std::shared_ptr<DeferredFramebuffer> gBuffer)
	{
		mGBuffer = gBuffer;
	}

	const glm::mat4& DeferredPointLight::Transform()
	{
		if (mTransformDirty)
		{
			mTransform = glm::mat4(0);
			mTransform[0][0] = mRadius;
			mTransform[1][1] = mRadius;
			mTransform[2][2] = mRadius;
			mTransform[3] = vec4(mPosition, 1);

			mTransformDirty = false;
		}

		return mTransform;
	}

	void DeferredPointLight::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
		mTransformDirty = true;
	}

	const glm::vec3& DeferredPointLight::Position() const
	{
		return mPosition;
	}

	void DeferredPointLight::SetRadius(float radius)
	{
		mRadius = radius;
		mTransformDirty = true;
	}

	float DeferredPointLight::Radius() const
	{
		return mRadius;
	}

	float DeferredPointLight::Intensity() const
	{
		return mIntensity;
	}

	void DeferredPointLight::SetIntensity(float intensity)
	{
		mIntensity = std::clamp(intensity, 0.f, 1.f);
	}

}