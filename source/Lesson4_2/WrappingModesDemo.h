#pragma once

#include "DrawableGameComponent.h"
#include "ShaderProgram.h"
#include "Game.h"

namespace Library
{
	struct VertexPositionTexture;
}

namespace Rendering
{
	class WrappingModesDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(WrappingModesDemo, Library::DrawableGameComponent)

	public:		
		WrappingModesDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		WrappingModesDemo(const WrappingModesDemo&) = delete;
		WrappingModesDemo& operator=(const WrappingModesDemo&) = delete;
		WrappingModesDemo(WrappingModesDemo&&) = delete;
		WrappingModesDemo& operator=(WrappingModesDemo&&) = delete;
		~WrappingModesDemo();

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};

		enum class WrappingMode
		{
			Repeat = 0,
			MirroredRepeat,
			ClampToEdge,
			ClampToBorder,
			End
		};

		void CreateIndexBuffer(const std::uint32_t* indices, std::uint32_t indexCount, GLuint& indexBuffer);
		void OnKey(int key, int scancode, int action, int mods);

		glm::mat4 mWorldMatrix{ 1 };
		std::vector<GLuint> mTextureSamplers;
		std::map<WrappingMode, GLuint> mTextureSamplersByWrappingMode;
		Library::ShaderProgram mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		GLint mWorldViewProjectionLocation = -1;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		WrappingMode mActiveWrappingMode = WrappingMode::Repeat;
		Library::Game::KeyboardHandler mKeyboardHandler;
	};
}