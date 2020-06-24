#pragma once

#include <limits>
#include "DrawableGameComponent.h"
#include "PointLightEffect.h"
#include "ColorHelper.h"

namespace Library
{
	class Light;
	class PointLight;
	class ProxyModel;
}

namespace Rendering
{
	class PointLightDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(PointLightDemo, DrawableGameComponent)

	public:		
		PointLightDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		PointLightDemo(const PointLightDemo&) = delete;
		PointLightDemo& operator=(const PointLightDemo&) = delete;
		PointLightDemo(PointLightDemo&&) = delete;
		PointLightDemo& operator=(PointLightDemo&&) = delete;
		~PointLightDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdatePointLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);

		inline static const float LightModulationRate = std::numeric_limits<unsigned char>::max();
		inline static const float LightMovementRate = 10.0f;
		inline static const float RadiusModulationRate = std::numeric_limits<unsigned char>::max();

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::White;
		PointLightEffect mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;		
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		std::unique_ptr<Library::Light> mAmbientLight;
		std::unique_ptr<Library::PointLight> mPointLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mSpecularPower = 25.0f;
	};
}
