#pragma once

#include "DrawableGameComponent.h"
#include "ShaderProgram.h"

namespace Library
{
	class Mesh;
	class Light;
	class DirectionalLight;
	class ProxyModel;
}

namespace Rendering
{
	class DiffuseLightingDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(DiffuseLightingDemo, Library::DrawableGameComponent)

	public:		
		DiffuseLightingDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		DiffuseLightingDemo(const DiffuseLightingDemo&) = delete;
		DiffuseLightingDemo& operator=(const DiffuseLightingDemo&) = delete;
		DiffuseLightingDemo(DiffuseLightingDemo&&) = delete;
		DiffuseLightingDemo& operator=(DiffuseLightingDemo&&) = delete;
		~DiffuseLightingDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};	

		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdateDirectionalLight(const Library::GameTime& gameTime);

		inline static const glm::vec2 LightRotationRate{ glm::two_pi<float>(), glm::two_pi<float>() };

		glm::mat4 mWorldMatrix{ 1 };
		Library::ShaderProgram mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		GLint mWorldViewProjectionLocation = -1;
		GLint mWorldLocation = -1;
		GLint mAmbientColorLocation = -1;
		GLint mLightColorLocation = -1;
		GLint mLightDirectionLocation = -1;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		std::unique_ptr<Library::Light> mAmbientLight;
		std::unique_ptr<Library::DirectionalLight> mDirectionalLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
	};
}
