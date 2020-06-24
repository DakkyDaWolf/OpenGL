#include "pch.h"
#include "GraphicsProgram.h"
#include "VertexDeclarations.h"
#include "VectorHelper.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "ProjectingLight.h"
#include "Camera.h"
#include "ProxyModel.h"

using namespace glm;
using namespace std;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(GraphicsProgram)

	GraphicsProgram::GraphicsProgram(Game& game, shared_ptr<Camera> camera) :
		DrawableGameComponent(game, move(camera))
	{
	}

	GraphicsProgram::~GraphicsProgram()
	{
		mGame->RemoveKeyboardHandler(mKeyboardHandler);
		glDeleteSamplers(1, &mTrilinearSampler);
		glDeleteTextures(1, &mNormalMap);
		glDeleteTextures(1, &mColorTexture);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteBuffers(1, &mShadowMappingVertexBuffer);
		glDeleteVertexArrays(1, &mShadowMappingVAO);
		glDeleteBuffers(1, &mDepthMappingVertexBuffer);
		glDeleteVertexArrays(1, &mDepthMappingVAO);
	}

	void GraphicsProgram::Initialize()
	{
		// Build the shader programs

		//vector<ShaderDefinition> shaders;
		//shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/NormalMappingDemo.vert"));
		//shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/NormalMappingDemo.frag"));
		//mNormalMappingEffect.BuildProgram(shaders);

		vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/ShadowcastingDirectional.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/ShadowcastingDirectional.frag"));
		mShadowcastEffect.BuildProgram(shaders);

		shaders.clear();
		shaders.push_back(ShaderDefinition(GL_VERTEX_SHADER, "Content/Effects/DepthPass.vert"));
		shaders.push_back(ShaderDefinition(GL_FRAGMENT_SHADER, "Content/Effects/DepthPass.frag"));
		mDepthPass.BuildProgram(shaders);
		
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

		// Create the depth mapping vertex array object
		glGenVertexArrays(1, &mDepthMappingVAO);
		glBindVertexArray(mDepthMappingVAO);

		// Create the depth mapping vertex buffer
		const VertexPosition depthMappingVertices[] =
		{
			VertexPosition(vec4(-1.0f, 0.1f, 0.0f, 1.0f)),
			VertexPosition(vec4(-1.0f, 2.1f, 0.0f, 1.0f)),
			VertexPosition(vec4(1.0f, 2.1f, 0.0f, 1.0f)),
			VertexPosition(vec4(1.0f, 0.1f, 0.0f, 1.0f))
		};

		VertexPosition::CreateVertexBuffer(depthMappingVertices, mDepthMappingVertexBuffer);
		mDepthPass.Initialize(mDepthMappingVAO);

		glBindVertexArray(0);


		// Create the normal mapping vertex array object
		glGenVertexArrays(1, &mShadowMappingVAO);
		glBindVertexArray(mShadowMappingVAO);

		// Create the normal mapping vertex buffer
		const VertexPositionTextureNormalTangentBinormal shadowMappingVertices[] =
		{
			VertexPositionTextureNormalTangentBinormal(vec4(-5.0f, 0.f, 0.0f, 1.0f), vec2(0.0f, 1.0f), Vector3Helper::Backward, Vector3Helper::Down, Vector3Helper::Right),
			VertexPositionTextureNormalTangentBinormal(vec4(-5.0f, 10.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), Vector3Helper::Backward, Vector3Helper::Down, Vector3Helper::Right),
			VertexPositionTextureNormalTangentBinormal(vec4(5.0f, 10.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), Vector3Helper::Backward, Vector3Helper::Down, Vector3Helper::Right),
			VertexPositionTextureNormalTangentBinormal(vec4(5.0f, 0.f, 0.0f, 1.0f), vec2(1.0f, 1.0f), Vector3Helper::Backward, Vector3Helper::Down, Vector3Helper::Right)
		};

		VertexPositionTextureNormalTangentBinormal::CreateVertexBuffer(shadowMappingVertices, mShadowMappingVertexBuffer);
		mShadowcastEffect.Initialize(mShadowMappingVAO);		

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

		mAmbientLight = make_shared<Light>(*mGame);
		mAmbientLight->SetColor(ColorHelper::Black);

		mDirectionalLight = make_shared<DirectionalLight>(*mGame);
		mSpotLight = make_shared<ProjectingLight>(*mGame);
		mSpotLight->Initialize();
		mSpotLight->SetPosition(0.0f, 5.0, 15.0f);
		mSpotLight->SetAttenuationRadius(500.f);
		mSpotLight->SetInnerAngle(1.f);
		mSpotLight->SetOuterAngle(0.95f);

		mProxyModel = make_shared<ProxyModel>(*mGame, mCamera, "Content/Models/DirectionalLightProxy.obj", 0.5f);
		mProxyModel->Initialize();
		mProxyModel->SetPosition(0.0f, 5.0, 15.0f);
		mProxyModel->ApplyRotation(rotate(mat4(1), half_pi<float>(), Vector3Helper::Up));

		mDog = make_shared<RenderedMesh>(*mGame, mCamera, "Content/Models/Wolf.obj");
		mDog->Initialize();
		mDog->SetPosition(0, 0, 7.f);
		mDog->SetAmbientLight(mAmbientLight);
		mDog->SetSpotLight(mSpotLight);
		//mDog->ApplyScale(10.f);

		// Attach the keyboard handler
		using namespace std::placeholders;
		mKeyboardHandler = bind(&GraphicsProgram::OnKey, this, _1, _2, _3, _4);
		mGame->AddKeyboardHandler(mKeyboardHandler);

		/*glGenFramebuffers(1, &mDepthMapFBOID);
		glGenTextures(1, &mDepthMapTextureID);
		glBindTexture(GL_TEXTURE_2D, mDepthMapTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mDepthMapW, mDepthMapH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	}

	void GraphicsProgram::Update(const GameTime& gameTime)
	{
		UpdateAmbientLight(gameTime);
		UpdateDirectionalLight(gameTime);
		UpdateSpecularLight(gameTime);

		mDog->Update(gameTime);
		mProxyModel->Update(gameTime);
	}

	void GraphicsProgram::Draw(const GameTime& gameTime)
	{
		if (!mShowShadowMapping)
		{
			glBindVertexArray(mShadowMappingVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mShadowMappingVertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

			mShadowcastEffect.Use();

			mat4 wvp = mCamera->ViewProjectionMatrix() * mWorldMatrix;
			mShadowcastEffect.WorldViewProjection() << wvp;
			mShadowcastEffect.World() << mWorldMatrix;
			mShadowcastEffect.AmbientColor() << mAmbientLight->Color();
			mShadowcastEffect.LightColor() << mSpotLight->Color();
			mShadowcastEffect.LightLookDirection() << mSpotLight->Direction();
			mShadowcastEffect.CameraPosition() << mCamera->Position();
			mShadowcastEffect.SpecularColor() << mSpecularColor;
			mShadowcastEffect.SpecularPower() << mSpecularPower;
			mShadowcastEffect.FogColor() << mFogColor;
			mShadowcastEffect.FogStart() << mFogStart;
			mShadowcastEffect.FogRange() << mFogRange;
			mShadowcastEffect.LightPosition() << mSpotLight->Position();
			mShadowcastEffect.LightFalloffRange() << mSpotLight->AttenuationRadius();
			mShadowcastEffect.LightInnerAngle() << mSpotLight->InnerAngle();
			mShadowcastEffect.LightOuterAngle() << mSpotLight->OuterAngle();


			glBindSampler(0, mTrilinearSampler);
			glBindSampler(1, mTrilinearSampler);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mColorTexture);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mNormalMap);

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);

			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		else
		{
			/*glBindVertexArray(mDepthMappingVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mDepthMappingVertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

			mDepthPass.Use();

			mat4 wvp = mSpotLight->ViewProjectionMatrix() * mWorldMatrix;
			mDepthPass.WorldViewProjection() << wvp;

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);

			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexCount), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			mDog->DepthTest(*mSpotLight);*/

		}

		mProxyModel->Draw(gameTime);
		mDog->Draw(gameTime);
	}

	float GraphicsProgram::AmbientLightIntensity() const
	{
		return mAmbientLight ? mAmbientLight->Color().r : 0.f;
	}

	bool GraphicsProgram::ShowingShadowMapping() const
	{
		return mShowShadowMapping;
	}

	void GraphicsProgram::UpdateAmbientLight(const GameTime& gameTime)
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

	void GraphicsProgram::UpdateDirectionalLight(const GameTime& gameTime)
	{
		static float directionalIntensity = 1.0f;
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Upddate directional light intensity		
		if (glfwGetKey(mGame->Window(), GLFW_KEY_HOME) && directionalIntensity < 1.0f)
		{
			directionalIntensity += elapsedTime;
			directionalIntensity = std::min(directionalIntensity, 1.0f);

			mDirectionalLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
			mSpotLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_END) && directionalIntensity > 0.0f)
		{
			directionalIntensity -= elapsedTime;
			directionalIntensity = std::max(directionalIntensity, 0.0f);

			mDirectionalLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
			mSpotLight->SetColor(vec4((vec3)directionalIntensity, 1.0f));
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_MINUS))
		{
			mSpotLight->SetAttenuationRadius(std::max(mSpotLight->AttenuationRadius() - elapsedTime * 50.f, 1.f));
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_EQUAL))
		{
			mSpotLight->SetAttenuationRadius(std::min(mSpotLight->AttenuationRadius() + elapsedTime * 50.f, 500.f));
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
			mSpotLight->ApplyRotation(lightRotationMatrix);
			mProxyModel->ApplyRotation(lightRotationMatrix);
		}
	}

	void GraphicsProgram::UpdateSpecularLight(const GameTime& gameTime)
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

	void GraphicsProgram::OnKey(int key, int /*scancode*/, int action, int /*mods*/)
	{
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			mShowShadowMapping = !mShowShadowMapping;
		}
	}

	void GraphicsProgram::RenderDepthMap()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBOID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMapTextureID, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, mDepthMapW, mDepthMapH);
		glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBOID);
		glClear(GL_DEPTH_BUFFER_BIT);
		//ConfigureShaderAndMatrices();
		//RenderScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 2. then render scene as normal with shadow mapping (using depth map)
		glViewport(0, 0, mGame->ScreenWidth(), mGame->ScreenHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ConfigureShaderAndMatrices();
		glBindTexture(GL_TEXTURE_2D, mDepthMapTextureID);
		//RenderScene();
	}
}