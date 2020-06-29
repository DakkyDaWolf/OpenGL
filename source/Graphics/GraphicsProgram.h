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

		bool ShowingShadowMapping() const;

	private:
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdateDirectionalLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);
		void OnKey(int key, int scancode, int action, int mods);
		void RenderDepthMap();

		inline static const glm::vec2 LightRotationRate{ glm::two_pi<float>(), glm::two_pi<float>() };
		inline static const float LightModulationRate{ std::numeric_limits<unsigned char>::max() };

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::Black;
		glm::vec4 mFogColor = Library::ColorHelper::CornflowerBlue;
		Library::ShadowcastedDirectionalEffect mShadowcastEffect;
		Library::DepthPass mDepthPass;

		GLuint mShadowMappingVAO = 0;
		GLuint mShadowMappingVertexBuffer = 0;
		GLuint mDepthMappingVAO = 0;
		GLuint mDepthMappingVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		std::shared_ptr<Library::Light> mAmbientLight;
		std::shared_ptr<Library::DirectionalLight> mDirectionalLight;
		std::shared_ptr<Library::ProjectingLight> mSpotLight;
		std::shared_ptr<Library::ProxyModel> mProxyModel;
		std::shared_ptr<Library::RenderedMesh> mDog;
		std::shared_ptr<Library::ScreenRect> mDebugRect;
		float mSpecularPower = 25.0f;
		float mFogStart = 20.0f;
		float mFogRange = 40.0f;
		GLuint mNormalMap = 0;
		GLuint mTrilinearSampler = 0;
		Library::Game::KeyboardHandler mKeyboardHandler;
		bool mShowShadowMapping = false;
	};
}
