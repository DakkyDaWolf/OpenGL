#pragma once

#include <limits>
#include "DrawableGameComponent.h"
#include "NormalMappingEffect.h"
#include "FogEffect.h"
#include "ColorHelper.h"
#include "Game.h"

namespace Library
{
	class Light;
	class DirectionalLight;
	class ProxyModel;
}

namespace Rendering
{
	class NormalMappingDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(NormalMappingDemo, Library::DrawableGameComponent)

	public:		
		NormalMappingDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		NormalMappingDemo(const NormalMappingDemo&) = delete;
		NormalMappingDemo& operator=(const NormalMappingDemo&) = delete;
		NormalMappingDemo(NormalMappingDemo&&) = delete;
		NormalMappingDemo& operator=(NormalMappingDemo&&) = delete;
		~NormalMappingDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdateDirectionalLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);
		void OnKey(int key, int scancode, int action, int mods);

		inline static const glm::vec2 LightRotationRate{ glm::two_pi<float>(), glm::two_pi<float>() };
		inline static const float LightModulationRate{ std::numeric_limits<unsigned char>::max() };

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::Black;
		glm::vec4 mFogColor = Library::ColorHelper::CornflowerBlue;
		NormalMappingEffect mNormalMappingEffect;
		FogEffect mFogEffect;
		GLuint mNormalMappingVAO = 0;
		GLuint mFogVAO = 0;
		GLuint mNormalMappingVertexBuffer = 0;
		GLuint mFogVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		std::unique_ptr<Library::Light> mAmbientLight;
		std::unique_ptr<Library::DirectionalLight> mDirectionalLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mSpecularPower = 25.0f;
		float mFogStart = 20.0f;
		float mFogRange = 40.0f;
		GLuint mNormalMap = 0;
		GLuint mTrilinearSampler = 0;
		Library::Game::KeyboardHandler mKeyboardHandler;
		bool mShowShadowMapping = true;
	};
}
