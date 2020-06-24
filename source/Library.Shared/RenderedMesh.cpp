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

using namespace glm;
using namespace std;

namespace Library
{

	RTTI_DEFINITIONS(RenderedMesh)

	RenderedMesh::RenderedMesh(Library::Game& game, std::shared_ptr<Camera> camera, const std::string& objectFilename, const std::string& textureFilename) :
		DrawableGameComponent(game, move(camera)), mObjectFilename(objectFilename), mTextureFilename(textureFilename), mTextured(textureFilename.empty())//,
		//MovableGameObject(game)
	{
	}

	RenderedMesh::~RenderedMesh()
	{
		glDeleteSamplers(1, &mTrilinearSampler);
		//glDeleteTextures(1, &mNormalMap);
		glDeleteTextures(1, &mColorTextureID);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void RenderedMesh::Reset()
	{
		mPosition = Vector3Helper::Zero;
		mDirection = Vector3Helper::Forward;
		mUp = Vector3Helper::Up;
		mRight = Vector3Helper::Right;
		mViewMatrixDataDirty = true;

		UpdateViewMatrix();
	}

	void RenderedMesh::UpdateViewMatrix()
	{
		vec3 target = mPosition + mDirection;
		mViewMatrix = lookAt(mPosition, target, mUp);
		mViewMatrixDataDirty = false;
	}

	void RenderedMesh::ApplyRotation(const glm::mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = (vec3)normalize(direction);

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = (vec3)normalize(up);

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
		mViewMatrixDataDirty = true;
	}

	void RenderedMesh::Initialize()
	{
		//load texture if specified
		if (!mTextureFilename.empty())
		{
			mColorTextureID = SOIL_load_OGL_texture(mTextureFilename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
			if (mColorTextureID == 0)
			{
				throw std::runtime_error("SOIL_load_OGL_texture() failed.");
			}
		}

		// Build the shader programs
		{
			vector<ShaderDefinition> shaders;
			shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/SpotLightEffect.vert"));
			shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/SpotLightEffect.frag"));
			mShaderProgram.BuildProgram(shaders);
		}
		{
			vector<ShaderDefinition> shaders;
			shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/SpotLightEffectAlbedo.vert"));
			shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/SpotLightEffectAlbedo.frag"));
			mShaderProgramAlbedo.BuildProgram(shaders);
		}
		{
			vector<ShaderDefinition> shaders;
			shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/DepthPass.vert"));
			shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/DepthPass.frag"));
			mShaderProgramDepth.BuildProgram(shaders);
		}

		// Load the model
		Model model(mObjectFilename, true);

		// Create the vertex and index buffers
		auto mesh = model.Meshes().at(0);
		VertexPositionTextureNormal::CreateVertexBuffer(*mesh, mVertexBuffer);
		VertexPosition::CreateVertexBuffer(*mesh, mVertexBufferDepth);
		mesh->CreateIndexBuffer(mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		// Create the vertex array objects
		glGenVertexArrays(1, &mVertexArrayObject);
		mShaderProgram.Initialize(mVertexArrayObject);
		mShaderProgramAlbedo.Initialize(mVertexArrayObject);
		glGenVertexArrays(1, &mVertexArrayObjectDepth);
		mShaderProgramDepth.Initialize(mVertexArrayObjectDepth);
		glBindVertexArray(0);
	}

	void RenderedMesh::Update(const Library::GameTime& gameTime)
	{
		if (mViewMatrixDataDirty)
		{
			UpdateViewMatrix();
		}
		DrawableGameComponent::Update(gameTime);
	}

	void RenderedMesh::Draw(const Library::GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		if (mTextured)
		{

			mShaderProgram.Use();

			mat4 wvp = mCamera->ViewProjectionMatrix() * ViewMatrix() * mWorldMatrix;
			mShaderProgram.WorldViewProjection() << wvp;
			mShaderProgram.World() << ViewMatrix() * mWorldMatrix;
			mShaderProgram.CameraPosition() << mCamera->Position();
			mShaderProgram.SpecularColor() << mSpecularColor;
			mShaderProgram.SpecularPower() << mSpecularPower;
			mShaderProgram.FogColor() << mFogColor;
			mShaderProgram.FogStart() << mFogStart;
			mShaderProgram.FogRange() << mFogRange;

			mShaderProgram.AmbientColor() << (mAmbientLight ? mAmbientLight->Color() : glm::vec4(1));

			mShaderProgram.LightColor() << (mSpotlight ? mSpotlight->Color() : glm::vec4(0));
			mShaderProgram.LightLookDirection() << (mSpotlight ? mSpotlight->Direction() : glm::vec3(1, 0, 0));
			mShaderProgram.LightPosition() << (mSpotlight ? mSpotlight->Position() : glm::vec3(0));
			mShaderProgram.LightFalloffRange() << (mSpotlight ? mSpotlight->AttenuationRadius() : 1.f);
			mShaderProgram.LightInnerAngle() << (mSpotlight ? mSpotlight->InnerAngle() : 0.25f);
			mShaderProgram.LightOuterAngle() << (mSpotlight ? mSpotlight->OuterAngle() : 0.3f);
			//mShaderProgram.EmissiveIntensity() << mEmissiveIntensity();

			glBindSampler(0, mTrilinearSampler);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mColorTextureID);
		}
		else
		{

			mShaderProgramAlbedo.Use();

			mat4 wvp = mCamera->ViewProjectionMatrix() * ViewMatrix() * mWorldMatrix;
			mShaderProgramAlbedo.WorldViewProjection() << wvp;
			mShaderProgramAlbedo.World() << ViewMatrix() * mWorldMatrix;
			mShaderProgramAlbedo.CameraPosition() << mCamera->Position();
			mShaderProgramAlbedo.SpecularColor() << mSpecularColor;
			mShaderProgramAlbedo.SpecularPower() << mSpecularPower;
			mShaderProgramAlbedo.FogColor() << mFogColor;
			mShaderProgramAlbedo.FogStart() << mFogStart;
			mShaderProgramAlbedo.FogRange() << mFogRange;

			mShaderProgramAlbedo.Albedo() << mAlbedo;
			
			mShaderProgramAlbedo.AmbientColor() << (mAmbientLight ? mAmbientLight->Color() : glm::vec4(1));
			
			mShaderProgramAlbedo.LightColor() << (mSpotlight ? mSpotlight->Color() : glm::vec4(0));
			mShaderProgramAlbedo.LightLookDirection() << (mSpotlight ? mSpotlight->Direction() : glm::vec3(1, 0, 0));
			mShaderProgramAlbedo.LightPosition() << (mSpotlight ? mSpotlight->Position() : glm::vec3(0));
			mShaderProgramAlbedo.LightFalloffRange() << (mSpotlight ? mSpotlight->AttenuationRadius() : 1.f);
			mShaderProgramAlbedo.LightInnerAngle() << (mSpotlight ? mSpotlight->InnerAngle() : 0.25f);
			mShaderProgramAlbedo.LightOuterAngle() << (mSpotlight ? mSpotlight->OuterAngle() : 0.3f);
			//mShaderProgramAlbedo.EmissiveIntensity() << mEmissiveIntensity();
		}

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	void RenderedMesh::DepthTest(ProjectingLight& lightSource)
	{
		glBindVertexArray(mVertexArrayObjectDepth);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferDepth);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgramDepth.Use();

		mat4 wvp = lightSource.ViewProjectionMatrix() * ViewMatrix() * mWorldMatrix;
		mShaderProgramDepth.WorldViewProjection() << wvp;

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	void RenderedMesh::SetAlbedo(glm::vec4 newColor)
	{
		mAlbedo = newColor;
	}

	void RenderedMesh::SetAmbientLight(std::shared_ptr<Light> newLight)
	{
		mAmbientLight = newLight;
	}

	void RenderedMesh::SetSpotLight(std::shared_ptr<ProjectingLight> newLight)
	{
		mSpotlight = newLight;
	}

	const glm::vec3& RenderedMesh::Position() const
	{
		return mPosition;
	}

	const glm::vec3& RenderedMesh::Direction() const
	{
		return mDirection;
	}

	const glm::vec3& RenderedMesh::Up() const
	{
		return mUp;
	}

	const glm::vec3& RenderedMesh::Right() const
	{
		return mRight;
	}

	const glm::mat4& RenderedMesh::ViewMatrix() const
	{
		return mViewMatrix;
	}

	void RenderedMesh::SetPosition(float x, float y, float z)
	{
		SetPosition(vec3(x, y, z));
	}

	void RenderedMesh::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
		mViewMatrixDataDirty = true;
	}
}