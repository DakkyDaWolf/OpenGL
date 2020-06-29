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
		DrawableGameComponent(game, move(camera)), mObjectFilename(objectFilename), mTextureFilename(textureFilename), mTextured(!textureFilename.empty()),
		MovableGameObject(game)
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
			mColorTextureID = SOIL_load_OGL_texture(mTextureFilename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
			if (mColorTextureID == 0)
			{
				throw std::runtime_error("SOIL_load_OGL_texture() failed.");
			}

			mAlbedo.a = 0;
			mTextured = true;
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
			shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/DepthPass.vert"));
			shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/DepthPass.frag"));
			mShaderProgramDepth.BuildProgram(shaders);
		}

		// Load the model
		Model model(mObjectFilename, true);

		// Create the vertex and index buffers
		auto mesh = model.Meshes().at(0);

		VertexPositionTextureNormal::CreateVertexBuffer(*mesh, mVertexBuffer);
		mesh->CreateIndexBuffer(mIndexBuffer);

		mIndexCount = mesh->Indices().size();

		// Create the vertex array objects
		glGenVertexArrays(1, &mVertexArrayObject);
		mShaderProgram.Initialize(mVertexArrayObject);
		glBindVertexArray(0);

		mWorldMatrix = mat4(1);

		Reset();
	}

	void RenderedMesh::Update(const Library::GameTime& gameTime)
	{
		if (mViewMatrixDataDirty)
		{
			UpdateViewMatrix();
		}
		MovableGameObject::Update(gameTime);
	}

	void RenderedMesh::Draw(const Library::GameTime& /*gameTime*/)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram.Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * Transform() * mWorldMatrix;
		mShaderProgram.WorldViewProjection() << wvp;
		mShaderProgram.World() << Transform() * mWorldMatrix;

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

		mShaderProgram.Albedo() << mAlbedo;

		glBindSampler(0, mTrilinearSampler);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mColorTextureID);


		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	void RenderedMesh::DepthTest(ProjectingLight& lightSource)
	{
		mShaderProgramDepth.Use();

		glViewport(0, 0, lightSource.DepthMapWidth(), lightSource.DepthMapHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, lightSource.FrameBuffer());

		mat4 wvp = lightSource.ViewProjectionMatrix() * Transform() * mWorldMatrix;
		mShaderProgramDepth.WorldViewProjection() << wvp;

		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, DrawableGameComponent::mGame->ScreenWidth(), DrawableGameComponent::mGame->ScreenHeight());
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
}