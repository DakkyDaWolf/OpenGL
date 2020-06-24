#pragma once

#include "DrawableGameComponent.h"
#include "ShaderProgram.h"

namespace Library
{
	class Mesh;
	class Light;
}

namespace Rendering
{
	class AmbientLightingDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(AmbientLightingDemo, Library::DrawableGameComponent)

	public:		
		AmbientLightingDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		AmbientLightingDemo(const AmbientLightingDemo&) = delete;
		AmbientLightingDemo& operator=(const AmbientLightingDemo&) = delete;
		AmbientLightingDemo(AmbientLightingDemo&&) = delete;
		AmbientLightingDemo& operator=(AmbientLightingDemo&&) = delete;
		~AmbientLightingDemo();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};

		void CreateVertexBuffer(const Library::Mesh& mesh, GLuint& vertexBuffer);
		void UpdateAmbientLight(const Library::GameTime& gameTime);

		glm::mat4 mWorldMatrix{ 1 };
		std::unique_ptr<Library::Light> mAmbientLight;
		Library::ShaderProgram mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		GLint mWorldViewProjectionLocation = -1;
		GLint mAmbientColorLocation = -1;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
	};
}
