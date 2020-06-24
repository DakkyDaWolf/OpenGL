#include "pch.h"
#include "SpotLightDemo.h"
#include "VectorHelper.h"
#include "ProxyModel.h"
#include "SpotLight.h"
#include "Camera.h"
#include "Game.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(SpotLightDemo)

	SpotLightDemo::SpotLightDemo(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	SpotLightDemo::~SpotLightDemo()
	{
		glDeleteTextures(1, &mColorTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteVertexArrays(1, &mVertexArrayObject);
	}

	void SpotLightDemo::Initialize()
	{
		// Build the shader program
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/SpotLightDemo.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/SpotLightDemo.frag"));
		mShaderProgram.BuildProgram(shaders);

		// Create the vertex buffer
		VertexPositionTextureNormal vertices[] =
		{
			VertexPositionTextureNormal(vec4(-1.0f, 0.1f, 0.0f, 1.0f), vec2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(vec4(-1.0f, 2.1f, 0.0f, 1.0f), vec2(0.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(vec4(1.0f, 2.1f, 0.0f, 1.0f), vec2(1.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(vec4(1.0f, 0.1f, 0.0f, 1.0f), vec2(1.0f, 1.0f), Vector3Helper::Backward)
		};

		VertexPositionTextureNormal::CreateVertexBuffer(vertices, mVertexBuffer);

		const uint32_t indices[] =
		{
			0, 2, 1,
			0, 3, 2
		};

		mIndexCount = std::size(indices);

		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndexCount, indices, GL_STATIC_DRAW);

		// Load the texture
		mColorTexture = SOIL_load_OGL_texture("Content/Textures/Checkerboard.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
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

		mSpotLight = make_unique<SpotLight>(*mGame);
		mSpotLight->SetRadius(500.0f);
		mSpotLight->SetPosition(5.0f, 0.0f, 10.0f);

		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Content/Models/SpotLightProxy.obj", 0.5f);
		mProxyModel->Initialize();
		mProxyModel->ApplyRotation(rotate(mat4(1), half_pi<float>(), Vector3Helper::Right));

		mWorldMatrix = scale(mat4(1), vec3(5.0f));
	}

	void SpotLightDemo::Update(const GameTime& gameTime)
	{
		UpdateAmbientLight(gameTime);
		UpdateSpotLight(gameTime);
		UpdateSpecularLight(gameTime);

		mProxyModel->Update(gameTime);
	}

	void SpotLightDemo::Draw(const GameTime& gameTime)
	{
		glBindVertexArray(mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		mShaderProgram.Use();

		mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
		mShaderProgram.WorldViewProjection() << wvp;
		mShaderProgram.World() << mWorldMatrix;
		mShaderProgram.AmbientColor() << mAmbientLight->Color();
		mShaderProgram.LightColor() << mSpotLight->Color();
		mShaderProgram.LightPosition() << mSpotLight->Position();
		mShaderProgram.LightRadius() << mSpotLight->Radius();
		mShaderProgram.LightLookAt() << mSpotLight->Direction();
		mShaderProgram.CameraPosition() << mCamera->Position();
		mShaderProgram.SpecularColor() << mSpecularColor;
		mShaderProgram.SpecularPower() << mSpecularPower;
		mShaderProgram.SpotLightInnerAngle() << mSpotLight->InnerAngle();
		mShaderProgram.SpotLightOuterAngle() << mSpotLight->OuterAngle();

		glBindTexture(GL_TEXTURE_2D, mColorTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		mProxyModel->Draw(gameTime);
	}

	void SpotLightDemo::UpdateAmbientLight(const GameTime& gameTime)
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

	void SpotLightDemo::UpdateSpotLight(const GameTime& gameTime)
	{
		static float directionalIntensity = 1.0f;
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Upddate directional light intensity		
		if (glfwGetKey(mGame->Window(), GLFW_KEY_HOME) && directionalIntensity < 1.0f)
		{
			directionalIntensity += elapsedTime;
			directionalIntensity = std::min(directionalIntensity, 1.0f);

			mSpotLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_END) && directionalIntensity > 0.0f)
		{
			directionalIntensity -= elapsedTime;
			directionalIntensity = std::max(directionalIntensity, 0.0f);

			mSpotLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
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
		mSpotLight->SetPosition(mSpotLight->Position() + movement);
		mProxyModel->SetPosition(mSpotLight->Position());

		// Update the light's radius
		if (glfwGetKey(mGame->Window(), GLFW_KEY_B))
		{
			float radius = mSpotLight->Radius() + LightModulationRate * elapsedTime;
			mSpotLight->SetRadius(radius);
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_N))
		{
			float radius = mSpotLight->Radius() - LightModulationRate * elapsedTime;
			radius = std::max(radius, 0.0f);
			mSpotLight->SetRadius(radius);
		}

		// Rotate spot light
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
			lightRotationMatrix = rotate(lightRotationMatrix, rotationAmount.y, mSpotLight->Right());
		}

		if (rotationAmount.x != 0.0f || rotationAmount.y != 0.0f)
		{
			mSpotLight->ApplyRotation(lightRotationMatrix);
			mProxyModel->ApplyRotation(lightRotationMatrix);
		}

		// Update inner and outer angles
		static float innerAngle = mSpotLight->InnerAngle();
		if (glfwGetKey(mGame->Window(), GLFW_KEY_Z) && innerAngle < 1.0f)
		{
			innerAngle += elapsedTime;
			innerAngle = std::min(innerAngle, 1.0f);

			mSpotLight->SetInnerAngle(innerAngle);
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_X) && innerAngle > 0.5f)
		{
			innerAngle -= elapsedTime;
			innerAngle = std::max(innerAngle, 0.5f);

			mSpotLight->SetInnerAngle(innerAngle);
		}

		static float outerAngle = mSpotLight->OuterAngle();
		if (glfwGetKey(mGame->Window(), GLFW_KEY_C) && outerAngle < 0.5f)
		{
			outerAngle += elapsedTime;
			outerAngle = std::min(outerAngle, 0.5f);

			mSpotLight->SetOuterAngle(outerAngle);
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_V) && outerAngle > 0.0f)
		{
			outerAngle -= elapsedTime;
			outerAngle = std::max(outerAngle, 0.0f);

			mSpotLight->SetOuterAngle(outerAngle);
		}
	}

	void SpotLightDemo::UpdateSpecularLight(const GameTime& gameTime)
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