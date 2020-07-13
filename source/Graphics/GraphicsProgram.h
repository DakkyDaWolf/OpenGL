#pragma once

#include <limits>
#include "DrawableGameComponent.h"
#include "NormalMappingEffect.h"
#include "ShadowcastedDirectionalEffect.h"
#include "DepthPass.h"
#include "FogEffect.h"
#include "ColorHelper.h"
#include "TextureProjector.h"
#include "RenderedMesh.h"
#include "ProjectingLight.h"
#include "DeferredScreen.h"
#include "DeferredFramebuffer.h"
#include "Game.h"
#include "ScreenRect.h"

namespace Library
{
	class Light;
	class DirectionalLight;
	class SpotLight;
	class ProxyModel;
}

namespace Rendering
{
	class GraphicsProgram final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(GraphicsProgram, Library::DrawableGameComponent)

	public:		
		GraphicsProgram(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		GraphicsProgram(const GraphicsProgram&) = delete;
		GraphicsProgram& operator=(const GraphicsProgram&) = delete;
		GraphicsProgram(GraphicsProgram&&) = delete;
		GraphicsProgram& operator=(GraphicsProgram&&) = delete;
		~GraphicsProgram();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		float AmbientLightIntensity() const;
		float DepthBias() const;
		std::shared_ptr<Library::MovableGameObject> ControlledObject();

		bool ShowingShadowMapping() const;
		std::string DeferredRenderingStatus() const;
		std::string DeferredDebugDisplayed() const;

	private:
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdateDirectionalLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);
		void UpdateControlledObjects(const Library::GameTime& gameTime);
		void UpdateShadowProperties(const Library::GameTime& gameTime);
		void OnKey(int key, int scancode, int action, int mods);


		inline static const glm::vec2 LightRotationRate{ glm::two_pi<float>(), glm::two_pi<float>() };
		inline static const float LightModulationRate{ std::numeric_limits<unsigned char>::max() };

		std::shared_ptr<Library::Light> mAmbientLight;
		std::shared_ptr<Library::DirectionalLight> mDirectionalLight;
		std::shared_ptr<Library::ProjectingLight> mSpotLight;
		std::shared_ptr<Library::ProxyModel> mProxyModel;
		std::shared_ptr<Library::RenderedMesh> mDog;
		std::shared_ptr<Library::RenderedMesh> mBuilding;
		std::shared_ptr<Library::RenderedMesh> mBuildingB;
		std::shared_ptr<Library::RenderedMesh> mCan;
		std::shared_ptr<Library::RenderedMesh> mFlashlight;
		std::shared_ptr<Library::ScreenRect> mDebugRect;
		std::shared_ptr<Library::ScreenRect> mDeferredDebugRect;
		std::shared_ptr<Library::DeferredScreen> mDeferredDisplay;

		std::shared_ptr<Library::DeferredFramebuffer> mGBuffer;

		const std::vector<std::function<void()>> gBufferSwitch = {
			[this]() { mDeferredDebugRect->SetTexture(mGBuffer->PositionID()); },
			[this]() { mDeferredDebugRect->SetTexture(mGBuffer->NormalID()); },
			[this]() { mDeferredDebugRect->SetTexture(mGBuffer->ColorID()); }
		};

		const std::vector<std::string> gBufferDataNames = {
			"Position",
			"Normals",
			"Color"
		};

		std::vector<std::shared_ptr<Library::RenderedMesh>> mMeshes;
		std::vector<std::shared_ptr<Library::MovableGameObject>> mControlledObjects;
		size_t mControlledObject = 0;
		float ItemRotationRate = glm::two_pi<float>();
		float ItemMovementRate = 15.f;

		float mDepthBias = 0.001f;

		Library::Game::KeyboardHandler mKeyboardHandler;
		bool mShowShadowMapping = false;
		bool mDeferredEnabled = false;
		int mShownGBuffer = 0;
	};
}
