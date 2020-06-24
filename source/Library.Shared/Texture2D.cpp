#include "pch.h"
#include "Texture2D.h"

using namespace std;
using namespace gsl;

namespace Library
{
	RTTI_DEFINITIONS(Texture2D)

	Texture2D::Texture2D(const GLuint textureID, GLuint width, GLuint height) :
		Texture(textureID), mWidth(width), mHeight(height)
	{
	}

	std::shared_ptr<Texture2D> Texture2D::CreateTexture2D(GLuint width, GLuint height, GLint mipLevels, GLenum colorFormat)
	{
		GLuint texture;
		glGenTextures(1, &texture);

		if (!texture)
		{
			throw runtime_error("glGenTextures() failed.");
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, max<GLint>(0, mipLevels - 1));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		return make_shared<Texture2D>(texture, width, height);
	}

	GLuint Texture2D::Width() const
	{
		return mWidth;
	}

	GLuint Texture2D::Height() const
	{
		return mHeight;
	}

	Rectangle Texture2D::Bounds() const
	{
		return Rectangle(0, 0, narrow<int32_t>(mWidth), narrow<int32_t>(mHeight));
	}
}