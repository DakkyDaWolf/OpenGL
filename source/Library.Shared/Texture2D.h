#pragma once

#include <gsl\gsl>
#include "Texture.h"
#include "Rectangle.h"

namespace Library
{
	class Texture2D final : public Texture
	{
		RTTI_DECLARATIONS(Texture2D, Texture)

	public:
		Texture2D(const GLuint textureID, GLuint width, GLuint height);
		Texture2D(const Texture2D&) = default;
		Texture2D& operator=(const Texture2D&) = default;
		Texture2D(Texture2D&&) = default;
		Texture2D& operator=(Texture2D&&) = default;
		~Texture2D() = default;

		static std::shared_ptr<Texture2D> CreateTexture2D(GLuint width, GLuint height, GLint mipLevels = 1, GLenum colorFormat = GL_RGBA16F);

		GLuint Width() const;
		GLuint Height() const;
		Rectangle Bounds() const;

	private:
		GLuint mWidth;
		GLuint mHeight;
	};
}