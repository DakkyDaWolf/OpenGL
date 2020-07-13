#pragma once
#include "ShaderProgram.h"

namespace Library
{
	struct VertexPositionTextureNormal;

	class DeferredEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(DeferredEffect, ShaderProgram)

			SHADER_VARIABLE_DECLARATION(SpecularPower)
			SHADER_VARIABLE_DECLARATION(WorldViewProjection)
			SHADER_VARIABLE_DECLARATION(World)

	public:
		DeferredEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};
	};
}