#pragma once
#include "ShaderProgram.h"

namespace Library
{

	class FlatTextureEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(FlatTextureEffect, ShaderProgram)

	public:
		FlatTextureEffect() = default;
		FlatTextureEffect(const FlatTextureEffect&) = delete;
		FlatTextureEffect& operator=(const FlatTextureEffect&) = delete;
		FlatTextureEffect(FlatTextureEffect&&) = delete;
		FlatTextureEffect& operator=(FlatTextureEffect&&) = delete;
		~FlatTextureEffect() = default;

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};
	};
}