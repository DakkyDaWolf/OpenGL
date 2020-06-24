#pragma once

#include "DrawableGameComponent.h"
#include "ShaderProgram.h"

namespace Rendering
{
	class ColoredTriangleDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(ColoredTriangleDemo, Library::DrawableGameComponent)

	public:		
		ColoredTriangleDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		ColoredTriangleDemo(const ColoredTriangleDemo&) = delete;
		ColoredTriangleDemo& operator=(const ColoredTriangleDemo&) = delete;
		ColoredTriangleDemo(ColoredTriangleDemo&&) = delete;
		ColoredTriangleDemo& operator=(ColoredTriangleDemo&&) = delete;
		~ColoredTriangleDemo();

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			Color = 1
		};

		Library::ShaderProgram mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
	};
}
