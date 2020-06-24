#pragma once

#include "ShaderProgram.h"

namespace Library
{
	struct VertexPositionTextureNormal;
}

namespace Rendering
{
	class EnvironmentMappingEffect : public Library::ShaderProgram
    {
		RTTI_DECLARATIONS(EnvironmentMappingEffect, ShaderProgram)

        SHADER_VARIABLE_DECLARATION(WorldViewProjection)
		SHADER_VARIABLE_DECLARATION(World)
		SHADER_VARIABLE_DECLARATION(AmbientColor)
		SHADER_VARIABLE_DECLARATION(EnvironmentColor)
		SHADER_VARIABLE_DECLARATION(ReflectionAmount)
		SHADER_VARIABLE_DECLARATION(CameraPosition)

    public:
        EnvironmentMappingEffect();

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