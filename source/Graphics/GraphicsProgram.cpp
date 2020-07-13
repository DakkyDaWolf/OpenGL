#include "pch.h"
#include "GraphicsProgram.h"
#include "VertexDeclarations.h"
#include "VectorHelper.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "ProjectingLight.h"
#include "Camera.h"
#include "ProxyModel.h"
#include "glm/gtx/transform.hpp"

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
	}

	void GraphicsProgram::Initialize()
	{
		mAmbientLight = make_shared<Light>(*mGame);
		mAmbientLight->SetColor(ColorHelper::LightGray);

		mDirectionalLight = make_shared<DirectionalLight>(*mGame);

		mSpotLight = make_shared<ProjectingLight>(*mGame);
		mSpotLight->Initialize();
		mSpotLight->SetPosition(0.0f, 10.0, 15.0f);
		mSpotLight->SetAttenuationRadius(5000.f);
		mSpotLight->SetInnerAngle(.95f);
		mSpotLight->SetOuterAngle(0.90f);
		mSpotLight->SetNearPlaneDistance(3.f);
		mSpotLight->SetFarPlaneDistance(500.f);
		mSpotLight->Rename("Spotlight");
		//mSpotLight->SetFieldOfView(30.f);
		//mSpotLight->ApplyRotation(rotate(pi<float>(), vec3(mSpotLight->Forward())));

		mProxyModel = make_shared<ProxyModel>(*mGame, mCamera, "Content/Models/DirectionalLightProxy.obj", 0.5f);
		mProxyModel->Initialize();
		mProxyModel->SetPosition(0.0f, 10.0, 15.0f);
		mProxyModel->ApplyRotation(rotate(mat4(1), half_pi<float>(), Vector3Helper::Up));

		mDog = make_shared<RenderedMesh>(*mGame, mCamera, "Content/Models/Wolf.obj", "Content/Textures/Wolf.png");
		mDog->Initialize();
		mDog->SetPosition(3.f, 0, 7.f);
		mDog->ApplyScale(10.f);
		//mDog->SetAlbedo(ColorHelper::Cyan);
		mDog->SetAmbientLight(mAmbientLight);
		mDog->SetSpotLight(mSpotLight);
		mMeshes.push_back(mDog);
		mDog->Rename("Wolf");

		mBuilding = make_shared<RenderedMesh>(*mGame, mCamera, "Content/Models/Building3.obj", "Content/Textures/Building.png");
		mBuilding->Initialize();
		mBuilding->SetPosition(20.f, 0, 0.f);
		mBuilding->ApplyScale(1.f);
		mBuilding->SetAmbientLight(mAmbientLight);
		mBuilding->SetSpotLight(mSpotLight);
		mMeshes.push_back(mBuilding);
		mBuilding->Rename("Tall Building");
		
		mBuildingB = make_shared<RenderedMesh>(*mGame, mCamera, "Content/Models/Building.obj", "Content/Textures/Building.png");
		mBuildingB->Initialize();
		mBuildingB->SetPosition(-20.f, 0, 3.f);
		mBuildingB->ApplyScale(1.f);
		mBuildingB->SetAmbientLight(mAmbientLight);
		mBuildingB->SetSpotLight(mSpotLight);
		mMeshes.push_back(mBuildingB);
		mBuildingB->Rename("Short Building");

		mFlashlight = make_shared<RenderedMesh>(*mGame, mCamera, "Content/Models/Flashlight.obj");
		mFlashlight->Initialize();
		mFlashlight->SetPosition(0.f, 0.f, 0.f);
		mFlashlight->ApplyRotation(rotate(pi<float>(), Vector3Helper::Up));
		mFlashlight->ApplyScale(0.5f);
		mFlashlight->SetAlbedo(ColorHelper::Black);
		mFlashlight->ApplyScale(1.f);
		//mMeshes.push_back(mFlashlight);
		mSpotLight->Adopt(*mFlashlight);

		mDebugRect = make_shared<ScreenRect>(*mGame);
		mDebugRect->Initialize(true);
		mDebugRect->SetDimensions(vec2(200.f, 200.f));
		mDebugRect->SetPosition(vec2(50.f, 50.f));
		mDebugRect->SetTexture(mSpotLight->DepthMapTexture());

		mGBuffer = make_shared<DeferredFramebuffer>(*mGame);
		mGBuffer->Initialize();

		mDeferredDisplay = make_shared<DeferredScreen>(*mGame, mCamera, mGBuffer, mSpotLight);
		mDeferredDisplay->Initialize();
		mDeferredDisplay->SetAmbientLight(mAmbientLight);

		mDeferredDebugRect = make_shared<ScreenRect>(*mGame);
		mDeferredDebugRect->Initialize(true);
		mDeferredDebugRect->SetDimensions(vec2(240.f, 180.f));
		mDeferredDebugRect->SetPosition(vec2(510.f, 50.f));
		mDeferredDebugRect->SetTexture(mGBuffer->PositionID());

		for (auto& mesh : mMeshes)
		{
			mesh->SetGBuffer(*mGBuffer);
		}
		mFlashlight->SetGBuffer(*mGBuffer);

		// Attach the keyboard handler
		using namespace std::placeholders;
		mKeyboardHandler = bind(&GraphicsProgram::OnKey, this, _1, _2, _3, _4);
		mGame->AddKeyboardHandler(mKeyboardHandler);

		mControlledObjects.push_back(mSpotLight);
		mControlledObjects.push_back(mDog);
		mControlledObjects.push_back(mBuilding);
		mControlledObjects.push_back(mBuildingB);
	}

	void GraphicsProgram::Update(const GameTime& gameTime)
	{
		UpdateAmbientLight(gameTime);
		UpdateDirectionalLight(gameTime);
		UpdateSpecularLight(gameTime);
		UpdateControlledObjects(gameTime);
		UpdateShadowProperties(gameTime);

		mDog->ApplyRotation(glm::rotate(glm::quarter_pi<float>() * gameTime.ElapsedGameTimeSeconds().count() / 2.f, Vector3Helper::Up));

		for (auto& mesh : mMeshes)
		{
			mesh->Update(gameTime);
		}

		mProxyModel->Update(gameTime);
	}

	void GraphicsProgram::Draw(const GameTime& gameTime)
	{
		mSpotLight->ClearBuffer();
		mGBuffer->ClearBuffer();

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glCullFace(GL_FRONT);

		for (auto& mesh : mMeshes)
		{
			mesh->DepthTest(*mSpotLight);
		}

		glCullFace(GL_BACK);

		for (auto& mesh : mMeshes)
		{
			mesh->DrawToGBuffer(gameTime);
		}

		if (!mShowShadowMapping)
		{
			for (auto& mesh : mMeshes)
			{
				mesh->Draw(gameTime);
			}
		}
		else
		{
			for (auto& mesh : mMeshes)
			{
				mesh->DrawShadowed(gameTime);
			}
		}

		mFlashlight->Draw(gameTime);
		if (mDeferredEnabled) mDeferredDisplay->Draw(gameTime);
		mDebugRect->Draw(gameTime);
		mDeferredDebugRect->Draw(gameTime);
	}

	float GraphicsProgram::AmbientLightIntensity() const
	{
		return mAmbientLight ? mAmbientLight->Color().r : 0.f;
	}

	float GraphicsProgram::DepthBias() const
	{
		return mDepthBias;
	}

	std::shared_ptr<Library::MovableGameObject> GraphicsProgram::ControlledObject()
	{
		return mControlledObjects.size() > 0 ? mControlledObjects[mControlledObject] : nullptr;
	}

	bool GraphicsProgram::ShowingShadowMapping() const
	{
		return mShowShadowMapping;
	}

	std::string GraphicsProgram::DeferredRenderingStatus() const
	{
		return mDeferredEnabled ? "Enabled" : "Disabled";
	}

	std::string GraphicsProgram::DeferredDebugDisplayed() const
	{
		return gBufferDataNames[mShownGBuffer];
	}

	void GraphicsProgram::UpdateAmbientLight(const GameTime& gameTime)
	{
		static float ambientIntensity = mAmbientLight->Color().r;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_EQUAL) && ambientIntensity < 1.0f)
		{
			ambientIntensity += gameTime.ElapsedGameTimeSeconds().count();
			ambientIntensity = std::min(ambientIntensity, 1.0f);

			mAmbientLight->SetColor(vec4((vec3)ambientIntensity, 1.0f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_MINUS) && ambientIntensity > 0.0f)
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

		if (glfwGetKey(mGame->Window(), GLFW_KEY_9))
		{
			mSpotLight->SetAttenuationRadius(std::max(mSpotLight->AttenuationRadius() - elapsedTime * 50.f, 1.f));
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_0))
		{
			mSpotLight->SetAttenuationRadius(std::min(mSpotLight->AttenuationRadius() + elapsedTime * 50.f, 500.f));
		}
	}

	void GraphicsProgram::UpdateSpecularLight(const GameTime& gameTime)
	{
		static float specularIntensity = 1.0f;

		if (glfwGetKey(mGame->Window(), GLFW_KEY_INSERT) && specularIntensity < 1.0f)
		{
			specularIntensity += gameTime.ElapsedGameTimeSeconds().count();
			specularIntensity = std::min(specularIntensity, 1.0f);

			for (auto& mesh : mMeshes)
			{
				mesh->mSpecularPower = specularIntensity;
			}
		}

		if (glfwGetKey(mGame->Window(), GLFW_KEY_DELETE) && specularIntensity > 0.0f)
		{
			specularIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			specularIntensity = std::max(specularIntensity, 0.0f);

			for (auto& mesh : mMeshes)
			{
				mesh->mSpecularPower = specularIntensity;
			}
		}
	}

	void GraphicsProgram::UpdateControlledObjects(const Library::GameTime& gameTime)
	{

		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Rotate directional light
		vec3 changeAmount = Vector3Helper::Zero;
		if (glfwGetKey(mGame->Window(), GLFW_KEY_LEFT))
		{
			changeAmount.x += elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_RIGHT))
		{
			changeAmount.x -= elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_UP))
		{
			changeAmount.z += elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_DOWN))
		{
			changeAmount.z -= elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_UP))
		{
			changeAmount.y += elapsedTime;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_PAGE_DOWN))
		{
			changeAmount.y -= elapsedTime;
		}

		if (!glfwGetKey(mGame->Window(), GLFW_KEY_LEFT_SHIFT))
		{
			changeAmount *= vec3(ItemRotationRate, 0, ItemRotationRate);

			mat4 rotationMatrix(1);
			if (changeAmount.x != 0)
			{
				rotationMatrix = rotate(rotationMatrix, changeAmount.x, Vector3Helper::Up);
			}

			if (changeAmount.z != 0)
			{
				rotationMatrix = rotate(rotationMatrix, changeAmount.z, vec3(mControlledObjects[mControlledObject]->Right()));
			}

			if (changeAmount.x != 0.0f || changeAmount.z != 0.0f)
			{
				mControlledObjects[mControlledObject]->ApplyRotation(rotationMatrix);
			}
		}
		else
		{
			changeAmount *= vec3(ItemMovementRate);

			vec3 vertical = Vector3Helper::Up;

			vec3 horizontal = vec3(mCamera->Rotation() * Vector4Helper::Left);
			horizontal.y = 0;
			horizontal /= horizontal.length();

			vec3 forward = vec3(mCamera->Rotation() * Vector4Helper::Forward);
			forward.y = 0;
			forward /= forward.length();

			vertical *= vec3(changeAmount.y);
			horizontal *= vec3(changeAmount.x);
			forward *= vec3(changeAmount.z);
			mControlledObjects[mControlledObject]->ApplyTranslation(horizontal + vertical + forward);
		}
	}

	void GraphicsProgram::UpdateShadowProperties(const Library::GameTime& gameTime)
	{
		float biasChange = 0.000001f * gameTime.TotalGameTimeSeconds().count();

		if (glfwGetKey(mGame->Window(), GLFW_KEY_LEFT_BRACKET))
		{
			mDepthBias -= biasChange;
			if (mDepthBias < 0) mDepthBias = 0;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_RIGHT_BRACKET))
		{
			mDepthBias += biasChange;
		}

		for (auto& mesh : mMeshes)
		{
			mesh->SetDepthBias(mDepthBias);
		}
	}

	void GraphicsProgram::OnKey(int key, int /*scancode*/, int action, int /*mods*/)
	{
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			mShowShadowMapping = !mShowShadowMapping;
		}

		if (key == GLFW_KEY_RIGHT_CONTROL && action == GLFW_PRESS)
		{
			mDeferredEnabled = !mDeferredEnabled;
		}

		if (key == GLFW_KEY_RIGHT_ALT && action == GLFW_PRESS)
		{
			++mShownGBuffer;
			if (mShownGBuffer >= 3) mShownGBuffer = 0;
			gBufferSwitch[mShownGBuffer]();
		}

		if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		{
			++mControlledObject;
			if (mControlledObject >= mControlledObjects.size()) mControlledObject = 0;
		}

	}
}