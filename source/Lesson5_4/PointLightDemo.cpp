#include "pch.h"
#include "PointLightDemo.h"
#include "Model.h"
#include "PointLight.h"
#include "ProxyModel.h"
#include "Camera.h"
#include "Game.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(PointLightDemo)

	PointLightDemo::PointLightDemo(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	PointLightDemo::~PointLightDemo()
	{
		glDeleteTextures(1, &mColorTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void PointLightDemo::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/PointLightDemo.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/PointLightDemo.frag"));
		mShaderProgram.BuildProgram(shaders);
		
		// Load the model
		Model model("Content/Models/Sphere.obj", true);

		// Create the vertex and index buffers
		auto mesh = model.Meshes().at(0);
		VertexPositionTextureNormal::CreateVertexBuffer(*mesh, mVertexBuffer);
		mesh->CreateIndexBuffer(mIndexBuffer);
		mIndexCount = mesh->Indices().size();
		
		// Load the texture
		mColorTexture = SOIL_load_OGL_texture("Content/Textures/Earthatday.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		if (mColorTexture == 0)
		{
			throw runtime_error("SOIL_load_OGL_texture() failed.");
		}

		// Create the vertex array object
		glGenVertexArrays(1, &mVertexArrayObject);
		mShaderProgram.Initialize(mVertexArrayObject);
		glBindVertexArray(0);

		mAmbientLight = make_unique<Light>(*mGame);
		mAmbientLight->SetColor(ColorHelper::Black);

		mPointLight = make_unique<PointLight>(*mGame);
		mPointLight->SetRadius(50.0f);
		mPointLight->SetPosition(5.0f, 0.0f, 10.0f);

		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Content/Models/PointLightProxy.obj", 0.5f);
		mProxyModel->Initialize();
	}

	void PointLightDemo::Update(const GameTime& gameTime)
	{
		UpdateAmbientLight(gameTime);
		UpdatePointLight(gameTime);
		UpdateSpecularLight(gameTime);

		mProxyModel->Update(gameTime);
	}

	void PointLightDemo::Draw(const GameTime& gameTime)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram.Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
		mShaderProgram.WorldViewProjection() << wvp;
		mShaderProgram.World() << mWorldMatrix;
		mShaderProgram.AmbientColor() << mAmbientLight->Color();
		mShaderProgram.LightColor() << mPointLight->Color();
		mShaderProgram.LightPosition() << mPointLight->Position();
		mShaderProgram.LightRadius() << mPointLight->Radius();
		mShaderProgram.CameraPosition() << mCamera->Position();
		mShaderProgram.SpecularColor() << mSpecularColor;
		mShaderProgram.SpecularPower() << mSpecularPower;

		glBindTexture(GL_TEXTURE_2D, mColorTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		mProxyModel->Draw(gameTime);
	}

	void PointLightDemo::UpdateAmbientLight(const GameTime& gameTime)
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

	void PointLightDemo::UpdatePointLight(const GameTime& gameTime)
	{
		static float directionalIntensity = 1.0f;
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Upddate directional light intensity		
		if (glfwGetKey(mGame->Window(), GLFW_KEY_HOME) && directionalIntensity < 1.0f)
		{
			directionalIntensity += elapsedTime;
			directionalIntensity = std::min(directionalIntensity, 1.0f);

			mPointLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_END) && directionalIntensity > 0.0f)
		{
			directionalIntensity -= elapsedTime;
			directionalIntensity = std::max(directionalIntensity, 0.0f);

			mPointLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}

		// Move point light
		vec3 movementAmount = Vector3Helper::Zero;
		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_4))
		{
			movementAmount.x = -1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_6))
		{
			movementAmount.x = 1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_9))
		{
			movementAmount.y = 1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_3))
		{
			movementAmount.y = -1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_8))
		{
			movementAmount.z = -1.0f;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_KP_2))
		{
			movementAmount.z = 1.0f;
		}

		vec3 movement = movementAmount * LightMovementRate * elapsedTime;
		mPointLight->SetPosition(mPointLight->Position() + movement);
		mProxyModel->SetPosition(mPointLight->Position());

		// Update the light's radius
		if (glfwGetKey(mGame->Window(), GLFW_KEY_V))
		{
			float radius = mPointLight->Radius() + LightModulationRate * elapsedTime;
			mPointLight->SetRadius(radius);
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_B))
		{
			float radius = mPointLight->Radius() - LightModulationRate * elapsedTime;
			radius = std::max(radius, 0.0f);
			mPointLight->SetRadius(radius);
		}
	}

	void PointLightDemo::UpdateSpecularLight(const GameTime& gameTime)
	{
		static float specularIntensity = 1.0f;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_INSERT) && specularIntensity < 1.0f)
		{
			specularIntensity += gameTime.ElapsedGameTimeSeconds().count();
			specularIntensity = std::min(specularIntensity, 1.0f);

			mSpecularColor = (vec4((vec3)specularIntensity, 1.0f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_DELETE) && specularIntensity > 0.0f)
		{
			specularIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			specularIntensity = std::max(specularIntensity, 0.0f);

			mSpecularColor = (vec4((vec3)specularIntensity, 1.0f));
		}

		static float specularPower = mSpecularPower;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_O) && specularPower < UCHAR_MAX)
		{
			specularPower += LightModulationRate * gameTime.ElapsedGameTimeSeconds().count();
			specularPower = std::min(specularPower, static_cast<float>(UCHAR_MAX));

			mSpecularPower = specularPower;
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_P) && specularPower > 0.0f)
		{
			specularPower -= LightModulationRate * gameTime.ElapsedGameTimeSeconds().count();
			specularPower = std::max(specularPower, 0.0f);

			mSpecularPower = specularPower;
		}
	}
}