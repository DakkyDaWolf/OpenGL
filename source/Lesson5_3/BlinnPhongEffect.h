#pragma once

#include "ShaderProgram.h"

namespace Library
{
	struct VertexPositionTextureNormal;
}

namespace Rendering
{
	class BlinnPhongEffect final : public Library::ShaderProgram
    {
		RTTI_DECLARATIONS(BlinnPhongEffect, Library::ShaderProgram)

        SHADER_VARIABLE_DECLARATION(WorldViewProjection)
		SHADER_VARIABLE_DECLARATION(World)
		SHADER_VARIABLE_DECLARATION(AmbientColor)
		SHADER_VARIABLE_DECLARATION(LightColor)
		SHADER_VARIABLE_DECLARATION(LightDirection)
		SHADER_VARIABLE_DECLARATION(CameraPosition)
		SHADER_VARIABLE_DECLARATION(SpecularColor)
		SHADER_VARIABLE_DECLARATION(SpecularPower)

    public:
        BlinnPhongEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum class VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2
		};
    };
}