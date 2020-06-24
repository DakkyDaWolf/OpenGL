#pragma once

#include "DrawableGameComponent.h"
#include "BlinnPhongEffect.h"
#include "ColorHelper.h"

namespace Library
{
	class Light;
	class DirectionalLight;
	class ProxyModel;
}

namespace Rendering
{
	class BlinnPhongDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(BlinnPhongDemo, Library::DrawableGameComponent)

	public:		
		BlinnPhongDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		BlinnPhongDemo(const BlinnPhongDemo&) = delete;
		BlinnPhongDemo& operator=(const BlinnPhongDemo&) = delete;
		BlinnPhongDemo(BlinnPhongDemo&&) = delete;
		BlinnPhongDemo& operator=(BlinnPhongDemo&&) = delete;
		~BlinnPhongDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdateDirectionalLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);

		static const glm::vec2 LightRotationRate;
		static const float LightModulationRate;

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::White;
		BlinnPhongEffect mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		std::unique_ptr<Library::Light> mAmbientLight;
		std::unique_ptr<Library::DirectionalLight> mDirectionalLight;		
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mSpecularPower = 25.0f;
	};
}
