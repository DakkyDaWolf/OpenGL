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
	class FilteringModesDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(FilteringModesDemo, Library::DrawableGameComponent)

	public:		
		FilteringModesDemo(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		FilteringModesDemo(const FilteringModesDemo&) = delete;
		FilteringModesDemo& operator=(const FilteringModesDemo&) = delete;
		FilteringModesDemo(FilteringModesDemo&&) = delete;
		FilteringModesDemo& operator=(FilteringModesDemo&&) = delete;
		~FilteringModesDemo();

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};

		enum class FilteringMode
		{
			Point = 0,
			Linear,
			PointMipMapPoint,
			LinearMipMapPoint,
			PointMipMapLinear,
			TriLinear,
			End
		};

		void CreateIndexBuffer(const std::uint32_t* indices, std::uint32_t indexCount, GLuint& indexBuffer);
		void OnKey(int key, int scancode, int action, int mods);

		glm::mat4 mWorldMatrix{ 1 };
		std::vector<GLuint> mTextureSamplers;
		std::map<FilteringMode, GLuint> mTextureSamplersByFilteringMode;
		Library::ShaderProgram mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		GLint mWorldViewProjectionLocation = -1;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		FilteringMode mActiveFilteringMode = FilteringMode::Point;
		Library::Game::KeyboardHandler mKeyboardHandler;
	};
}