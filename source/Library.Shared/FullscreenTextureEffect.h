#pragma once
#include "ShaderProgram.h"

namespace Library
{

	class FullscreenTextureEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(FullscreenTextureEffect, ShaderProgram)

	public:
		FullscreenTextureEffect() = default;
		FullscreenTextureEffect(const FullscreenTextureEffect&) = delete;
		FullscreenTextureEffect& operator=(const FullscreenTextureEffect&) = delete;
		FullscreenTextureEffect(FullscreenTextureEffect&&) = delete;
		FullscreenTextureEffect& operator=(FullscreenTextureEffect&&) = delete;
		~FullscreenTextureEffect() = default;

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};
	};
}