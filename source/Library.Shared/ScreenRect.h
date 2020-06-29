#pragma once
#include "Rectangle.h"
#include "DrawableGameComponent.h"
#include "TextureRectEffect.h"
#include "GL/gl3w.h"

namespace Library
{

	class ScreenRect final : public DrawableGameComponent, public Rectangle
	{
		RTTI_DECLARATIONS(ScreenRect, DrawableGameComponent)

	public:
		ScreenRect(Game& game, const std::string& filename = "");
		ScreenRect(const ScreenRect&) = default;
		ScreenRect& operator=(const ScreenRect&) = default;
		ScreenRect(ScreenRect&&) = default;
		ScreenRect& operator=(ScreenRect&&) = default;
		virtual ~ScreenRect() = default;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		void SetTexture(GLuint textureID);

		void SetPosition(const glm::vec2& position);
		void SetDimensions(const glm::vec2& dimensions);

		const glm::mat4& ScreenspaceTransform();

	private:
		glm::mat4 mScreenspaceTransform{ 1 };
		glm::vec2 mPosition{ 0,0 };
		glm::vec2 mDimensions{ 1,1 };
		bool mScreenspaceTransformDirty = true;

		TextureRectEffect mShaderProgram;
		GLuint mTextureID = 0;
		GLuint mTrilinearSampler = 0;
		GLuint mRectVertexBuffer = 0;
		GLuint mRectVAO = 0;
		GLuint mIndexBuffer = 0;
		GLuint mIndexCount = 0;

		std::string mTextureFilename;
	};

}