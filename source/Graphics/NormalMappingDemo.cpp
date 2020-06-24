#include "pch.h"
#include "NormalMappingDemo.h"
#include "VertexDeclarations.h"
#include "VectorHelper.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "ProxyModel.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(NormalMappingDemo)

	NormalMappingDemo::NormalMappingDemo(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	NormalMappingDemo::~NormalMappingDemo()
	{
		mGame->RemoveKeyboardHandler(mKeyboardHandler);
		glDeleteSamplers(1, &mTrilinearSampler);
		glDeleteTextures(1, &mNormalMap);
		glDeleteTextures(1, &mColorTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mNormalMappingVertexBuffer);
		glDeleteBuffers(1, &mFogVertexBuffer);
		glDeleteVertexArrays(1, &mFogVAO);
		glDeleteVertexArrays(1, &mNormalMappingVAO);
	}

	void NormalMappingDemo::Initialize()
	{
		// Build the shader programs
		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/NormalMappingDemo.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/NormalMappingDemo.frag"));
		mNormalMappingEffect.BuildProgram(shaders);

		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/FogDemo.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/FogDemo.frag"));
		mFogEffect.BuildProgram(shaders);

		// Load the color texture
		mColorTexture = SOIL_load_OGL_texture("Content/Textures/Blocks_COLOR.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		if (mColorTexture == 0)
		{
			throw runtime_error("SOIL_load_OGL_texture() failed.");
		}

		// Load the normal map
		mNormalMap = SOIL_load_OGL_texture("Content/Textures/Blocks_NORM.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		if (mNormalMap == 0)
		{
			throw runtime_error("SOIL_load_OGL_texture() failed.");
		}

		// Create the trilinear texture sampler
		glGenSamplers(1, &mTrilinearSampler);
		glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(mTrilinearSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Create the normal mapping vertex array object
		glGenVertexArrays(1, &mNormalMappingVAO);
		glBindVertexArray(mNormalMappingVAO);

		// Create the normal mapping vertex buffer
		const VertexPositionTextureNormalTangentBinormal normalMappingVertices[] =
		{
			VertexPositionTextureNormalTangentBinormal(vec4(-1.0f, 0.1f, 0.0f, 1.0f), vec2(0.0f, 1.0f), Vector3Helper::Backward, Vector3Helper::Down, Vector3Helper::Right),
			VertexPositionTextureNormalTangentBinormal(vec4(-1.0f, 2.1f, 0.0f, 1.0f), vec2(0.0f, 0.0f), Vector3Helper::Backward, Vector3Helper::Down, Vector3Helper::Right),
			VertexPositionTextureNormalTangentBinormal(vec4(1.0f, 2.1f, 0.0f, 1.0f), vec2(1.0f, 0.0f), Vector3Helper::Backward, Vector3Helper::Down, Vector3Helper::Right),
			VertexPositionTextureNormalTangentBinormal(vec4(1.0f, 0.1f, 0.0f, 1.0f), vec2(1.0f, 1.0f), Vector3Helper::Backward, Vector3Helper::Down, Vector3Helper::Right)
		};

		VertexPositionTextureNormalTangentBinormal::CreateVertexBuffer(normalMappingVertices, mNormalMappingVertexBuffer);
		mNormalMappingEffect.Initialize(mNormalMappingVAO);

		// Create the fog vertex array object
		glGenVertexArrays(1, &mFogVAO);
		glBindVertexArray(mFogVAO);
		
		// Create the fog vertex buffer
		VertexPositionTextureNormal fogVertices[] =
		{
			VertexPositionTextureNormal(vec4(-1.0f, 0.1f, 0.0f, 1.0f), vec2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(vec4(-1.0f, 2.1f, 0.0f, 1.0f), vec2(0.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(vec4(1.0f, 2.1f, 0.0f, 1.0f), vec2(1.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(vec4(1.0f, 0.1f, 0.0f, 1.0f), vec2(1.0f, 1.0f), Vector3Helper::Backward)
		};

		VertexPositionTextureNormal::CreateVertexBuffer(fogVertices, mFogVertexBuffer);
		mFogEffect.Initialize(mFogVAO);
		glBindVertexArray(0);
		
		// Create the index buffer
		const uint32_t indices[] =
		{
			0, 2, 1,
			0, 3, 2
		};

		mIndexCount = std::size(indices);

		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mIndexCount, indices, GL_STATIC_DRAW);

		mAmbientLight = make_unique<Light>(*mGame);
		mAmbientLight->SetColor(ColorHelper::Black);

		mDirectionalLight = make_unique<DirectionalLight>(*mGame);

		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Content/Models/DirectionalLightProxy.obj", 0.5f);
		mProxyModel->Initialize();
		mProxyModel->SetPosition(10.0f, 0.0, 0.0f);
		mProxyModel->ApplyRotation(rotate(mat4(1), half_pi<float>(), Vector3Helper::Up));

		mWorldMatrix = scale(mat4(1), vec3(5.0f));

		// Attach the keyboard handler
		using namespace std::placeholders;
		mKeyboardHandler = bind(&NormalMappingDemo::OnKey, this, _1, _2, _3, _4);
		mGame->AddKeyboardHandler(mKeyboardHandler);
	}

	void NormalMappingDemo::Update(const GameTime& gameTime)
	{
		UpdateAmbientLight(gameTime);
		UpdateDirectionalLight(gameTime);
		UpdateSpecularLight(gameTime);

		mProxyModel->Update(gameTime);
	}

	void NormalMappingDemo::Draw(const GameTime& gameTime)
	{
		if (mShowShadowMapping)
		{
			glBindVertexArray(mNormalMappingVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mNormalMappingVertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

			mNormalMappingEffect.Use();

			mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
			mNormalMappingEffect.WorldViewProjection() << wvp;
			mNormalMappingEffect.World() << mWorldMatrix;
			mNormalMappingEffect.AmbientColor() << mAmbientLight->Color();
			mNormalMappingEffect.LightColor() << mDirectionalLight->Color();
			mNormalMappingEffect.LightDirection() << mDirectionalLight->Direction();
			mNormalMappingEffect.CameraPosition() << mCamera->Position();
			mNormalMappingEffect.SpecularColor() << mSpecularColor;
			mNormalMappingEffect.SpecularPower() << mSpecularPower;
			mNormalMappingEffect.FogColor() << mFogColor;
			mNormalMappingEffect.FogStart() << mFogStart;
			mNormalMappingEffect.FogRange() << mFogRange;

			glBindSampler(0, mTrilinearSampler);
			glBindSampler(1, mTrilinearSampler);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mColorTexture);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mNormalMap);
		}
		else
		{
			glBindVertexArray(mFogVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mFogVertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

			mFogEffect.Use();

			mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
			mFogEffect.WorldViewProjection() << wvp;
			mFogEffect.World() << mWorldMatrix;
			mFogEffect.AmbientColor() << mAmbientLight->Color();
			mFogEffect.LightColor() << mDirectionalLight->Color();
			mFogEffect.LightDirection() << mDirectionalLight->Direction();
			mFogEffect.CameraPosition() << mCamera->Position();
			mFogEffect.SpecularColor() << mSpecularColor;
			mFogEffect.SpecularPower() << mSpecularPower;
			mFogEffect.FogColor() << mFogColor;
			mFogEffect.FogStart() << mFogStart;
			mFogEffect.FogRange() << mFogRange;

			glBindSampler(0, mTrilinearSampler);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mColorTexture);
		}

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		mProxyModel->Draw(gameTime);
	}

	void NormalMappingDemo::UpdateAmbientLight(const GameTime& gameTime)
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

	void NormalMappingDemo::UpdateDirectionalLight(const GameTime& gameTime)
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

	void NormalMappingDemo::UpdateSpecularLight(const GameTime& gameTime)
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

	void NormalMappingDemo::OnKey(int key, int /*scancode*/, int action, int /*mods*/)
	{
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			mShowShadowMapping = !mShowShadowMapping;
		}
	}
}