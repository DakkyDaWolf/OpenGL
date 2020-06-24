#pragma once

#include <limits>
#include "DrawableGameComponent.h"
#include "FogEffect.h"
#include "ColorHelper.h"

namespace Library
{
	class Light;
	class DirectionalLight;
	class ProxyModel;
}

namespace Rendering
{
	class FogDemo : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(FogDemo, Library::DrawableGameComponent)

	public:
		FogDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		FogDemo(const FogDemo&) = delete;
		FogDemo& operator=(const FogDemo&) = delete;
		FogDemo(FogDemo&&) = delete;
		FogDemo& operator=(FogDemo&&) = delete;
		~FogDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdateDirectionalLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);

		inline static const glm::vec2 LightRotationRate{ glm::two_pi<float>(), glm::two_pi<float>() };
		inline static const float LightModulationRate{ std::numeric_limits<unsigned char>::max() };

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::White;
		glm::vec4 mFogColor = Library::ColorHelper::CornflowerBlue;
		FogEffect mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		std::unique_ptr<Library::Light> mAmbientLight;
		std::unique_ptr<Library::DirectionalLight> mDirectionalLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mSpecularPower = 25.0f;
		float mFogStart = 20.0f;
		float mFogRange = 40.0f;
	};
}