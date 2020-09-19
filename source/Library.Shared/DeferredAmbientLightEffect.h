#pragma once
#include "ShaderProgram.h"

namespace Library
{
	struct VertexPositionTexture;

	class DeferredAmbientLightEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(DeferredAmbientLightEffect, ShaderProgram)

		SHADER_VARIABLE_DECLARATION(LightColor)

	public:
		DeferredAmbientLightEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};
	};
}