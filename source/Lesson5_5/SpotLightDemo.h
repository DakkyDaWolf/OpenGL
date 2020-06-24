#pragma once

#include "DrawableGameComponent.h"
#include "SpotLightEffect.h"
#include "ColorHelper.h"

namespace Library
{
	class Light;
	class SpotLight;
	class ProxyModel;
}

namespace Rendering
{
	class SpotLightDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(SpotLightDemo, Library::DrawableGameComponent)

	public:		
		SpotLightDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		SpotLightDemo(const SpotLightDemo&) = delete;
		SpotLightDemo& operator=(const SpotLightDemo&) = delete;
		SpotLightDemo(SpotLightDemo&&) = delete;
		SpotLightDemo& operator=(SpotLightDemo&&) = delete;
		~SpotLightDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdateSpotLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);

		inline static const float LightModulationRate = std::numeric_limits<unsigned char>::max();
		inline static const float LightMovementRate = 10.0f;
		inline static const glm::vec2 LightRotationRate = glm::vec2(glm::two_pi<float>(), glm::two_pi<float>());

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::White;
		SpotLightEffect mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		std::unique_ptr<Library::Light> mAmbientLight;
		std::unique_ptr<Library::SpotLight> mSpotLight;		
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mSpecularPower = 25.0;
	};
}