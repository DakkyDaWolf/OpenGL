#pragma once
#include "ShaderProgram.h"

namespace Library
{
	struct VertexPositionTexture;

	class TextureRectEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(TextureRectEffect, ShaderProgram)

		SHADER_VARIABLE_DECLARATION(ScreenspaceProjection)

	public:
		TextureRectEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};
	};
}