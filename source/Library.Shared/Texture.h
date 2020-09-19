#pragma once

#include <GL/gl3w.h>
#include "RTTI.h"

namespace Library
{
	class Texture : public RTTI
	{
		RTTI_DECLARATIONS(Texture, RTTI)

	public:
		Texture(const Texture&) = default;
		Texture(const std::string& fileName);
		Texture& operator=(const Texture&) = default;
		Texture(Texture&&) = default;
		Texture& operator=(Texture&&) = default;
		virtual ~Texture() = default;

		GLuint TextureID() const;

	protected:
		Texture(GLuint shaderResource);

		GLuint mShaderResource;
	};
}

#include "Texture.inl"