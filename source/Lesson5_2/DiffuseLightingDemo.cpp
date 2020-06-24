#include "pch.h"
#include "DiffuseLightingDemo.h"
#include "Model.h"
#include "VertexDeclarations.h"
#include "VectorHelper.h"
#include "ColorHelper.h"
#include "ProxyModel.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Game.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(DiffuseLightingDemo)


	DiffuseLightingDemo::DiffuseLightingDemo(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, camera)
	{
	}

	DiffuseLightingDemo::~DiffuseLightingDemo()
	{
		glDeleteTextures(1, &mColorTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void DiffuseLightingDemo::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/DiffuseLightingDemo.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/DiffuseLightingDemo.frag"));
		mShaderProgram.BuildProgram(shaders);
		
		// Load the model
		Model model("Content/Models/Sphere.obj", true);

		// Create the vertex and index buffers
		auto mesh = model.Meshes().at(0);
		VertexPositionTextureNormal::CreateVertexBuffer(*mesh, mVertexBuffer);
		mesh->CreateIndexBuffer(mIndexBuffer);
		mIndexCount = mesh->Indices().size();
		
		mWorldViewProjectionLocation = glGetUniformLocation(mShaderProgram.Program(), "WorldViewProjection");
		if (mWorldViewProjectionLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform location.");
		}

		mWorldLocation = glGetUniformLocation(mShaderProgram.Program(), "World");
		if (mWorldLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform location.");
		}

		mAmbientColorLocation = glGetUniformLocation(mShaderProgram.Program(), "AmbientColor");
		if (mAmbientColorLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform location.");
		}

		mLightColorLocation = glGetUniformLocation(mShaderProgram.Program(), "LightColor");
		if (mLightColorLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform location.");
		}

		mLightDirectionLocation = glGetUniformLocation(mShaderProgram.Program(), "LightDirection");
		if (mLightDirectionLocation == -1)
		{
			throw runtime_error("glGetUniformLocation() did not find uniform location.");
		}

		// Load the texture
		mColorTexture = SOIL_load_OGL_texture("Content/Textures/EarthComposite.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		if (mColorTexture == 0)
		{
			throw runtime_error("SOIL_load_OGL_texture() failed.");
		}

		// Create the vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
		glBindVertexArray(mVertexArrayObject);

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));

		glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Normal), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
		glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Normal));

		glBindVertexArray(0);

		mAmbientLight = make_unique<Light>(*mGame);
		mAmbientLight->SetColor(ColorHelper::Black);

		mDirectionalLight = make_unique<DirectionalLight>(*mGame);

		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Content/Models/DirectionalLightProxy.obj", 0.5f);
		mProxyModel->Initialize();
		mProxyModel->SetPosition(10.0f, 0.0, 0.0f);
		mProxyModel->ApplyRotation(rotate(mat4(1), radians(90.0f), Vector3Helper::Up));
	}

	void DiffuseLightingDemo::Update(const GameTime& gameTime)
	{
		UpdateAmbientLight(gameTime);
		UpdateDirectionalLight(gameTime);

		mProxyModel->Update(gameTime);
	}

	void DiffuseLightingDemo::Draw(const GameTime& gameTime)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		glUseProgram(mShaderProgram.Program());

		mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
		glUniformMatrix4fv(mWorldViewProjectionLocation, 1, GL_FALSE, &wvp[0][0]);
		glUniformMatrix4fv(mWorldLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glUniform4fv(mAmbientColorLocation, 1, &mAmbientLight->Color()[0]);
		glUniform4fv(mLightColorLocation, 1, &mDirectionalLight->Color()[0]);
		glUniform3fv(mLightDirectionLocation, 1, &mDirectionalLight->Direction()[0]);
		glBindTexture(GL_TEXTURE_2D, mColorTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		mProxyModel->Draw(gameTime);
	}

	void DiffuseLightingDemo::UpdateAmbientLight(const GameTime& gameTime)
	{
		static float ambientIntensity = mAmbientLight->Color().r;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_UP) && ambientIntensity < 1.0f)
		{
			ambientIntensity += gameTime.ElapsedGameTimeSeconds().count();
			ambientIntensity = std::min(ambientIntensity, 1.0f);

			mAmbientLight->SetColor(vec4((vec3)ambientIntensity, 1.0f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_DOWN) && ambientIntensity > 0.0f)
		{
			ambientIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			ambientIntensity = std::max(ambientIntensity, 0.0f);

			mAmbientLight->SetColor(vec4((vec3)ambientIntensity, 1.0f));
		}
	}

	void DiffuseLightingDemo::UpdateDirectionalLight(const GameTime& gameTime)
	{
		static float directionalIntensity = 1.0f;
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Upddate directional light intensity		
		if (glfwGetKey(mGame->Window(), GLFW_KEY_HOME) && directionalIntensity < 1.0f)
		{
			directionalIntensity += elapsedTime;
			directionalIntensity = std::min(directionalIntensity, 1.0f);

			mDirectionalLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_END) && directionalIntensity > 0.0f)
		{
			directionalIntensity -= elapsedTime;
			directionalIntensity = std::max(directionalIntensity, 0.0f);

			mDirectionalLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}

		// Rotate directional light
		vec2 rotationAmount = Vector2Helper::Zero;
		if (glfwGetKey(mGame->Window(), GLFW_KEY_LEFT))
		{
			rotationAmount.x += LightRotationRate.x * elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_RIGHT))
		{
			rotationAmount.x -= LightRotationRate.x * elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_UP))
		{
			rotationAmount.y += LightRotationRate.y * elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_DOWN))
		{
			rotationAmount.y -= LightRotationRate.y * elapsedTime;
		}

		mat4 lightRotationMatrix(1);
		if (rotationAmount.x != 0)
		{
			lightRotationMatrix = rotate(mat4(1), rotationAmount.x, Vector3Helper::Up);
		}

		if (rotationAmount.y != 0)
		{
			lightRotationMatrix = rotate(lightRotationMatrix, rotationAmount.y, mDirectionalLight->Right());
		}

		if (rotationAmount.x != 0.0f || rotationAmount.y != 0.0f)
		{
			mDirectionalLight->ApplyRotation(lightRotationMatrix);
			mProxyModel->ApplyRotation(lightRotationMatrix);
		}
	}
}