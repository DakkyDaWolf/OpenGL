#pragma once

#include "DrawableGameComponent.h"
#include "EnvironmentMappingEffect.h"
#include "ColorHelper.h"

namespace Library
{
	class Light;
}

namespace Rendering
{
	class EnvironmentMappingDemo : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(EnvironmentMappingDemo, Library::DrawableGameComponent)

	public:		
		EnvironmentMappingDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		EnvironmentMappingDemo(const EnvironmentMappingDemo&) = delete;
		EnvironmentMappingDemo& operator=(const EnvironmentMappingDemo&) = delete;
		EnvironmentMappingDemo(EnvironmentMappingDemo&&) = delete;
		EnvironmentMappingDemo& operator=(EnvironmentMappingDemo&&) = delete;
		~EnvironmentMappingDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdateReflectionAmount(const Library::GameTime& gameTime);
		
		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mEnvironmentColor = Library::ColorHelper::White;
		EnvironmentMappingEffect mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		GLuint mEnvironmentMap = 0;
		std::unique_ptr<Library::Light> mAmbientLight;
		float mReflectionAmount = 1.0f;
		GLuint mColorTextureSampler = 0;
		GLuint mEnvironmentMapSampler = 0;
	};
}
