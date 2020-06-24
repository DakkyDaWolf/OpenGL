#pragma once

#include "DrawableGameComponent.h"
#include "BasicEffect.h"

namespace Library
{
	class Mesh;
}
namespace Rendering
{
	class ModelDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(ModelDemo, Library::DrawableGameComponent)

	public:		
		ModelDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		ModelDemo(const ModelDemo&) = delete;
		ModelDemo& operator=(const ModelDemo&) = delete;
		ModelDemo(ModelDemo&&) = delete;
		ModelDemo& operator=(ModelDemo&&) = delete;
		~ModelDemo();

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			Color = 1
		};		

		glm::mat4 mWorldMatrix{ 1 };
		Library::ShaderProgram mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		GLint mWorldViewProjectionLocation = -1;		
		std::size_t mIndexCount = 0;
	};
}