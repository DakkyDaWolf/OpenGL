#pragma once

#include "DrawableGameComponent.h"
#include "ShaderProgram.h"
#include "Game.h"

namespace Library
{
	class Mesh;
}

namespace Rendering
{
	class TexturedModelDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(TexturedModelDemo, Library::DrawableGameComponent)

	public:		
		TexturedModelDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		TexturedModelDemo(const TexturedModelDemo&) = delete;
		TexturedModelDemo& operator=(const TexturedModelDemo&) = delete;
		TexturedModelDemo(TexturedModelDemo&&) = delete;
		TexturedModelDemo& operator=(TexturedModelDemo&&) = delete;
		~TexturedModelDemo();

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};		

		void OnKey(int key, int scancode, int action, int mods);

		glm::mat4 mWorldMatrix{ 1 };
		Library::Game::KeyboardHandler mKeyboardHandler;
		Library::ShaderProgram mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		GLint mWorldViewProjectionLocation = -1;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		GLuint mAltTexture = 0;
		GLuint mActiveTexture = 0;
	};
}
