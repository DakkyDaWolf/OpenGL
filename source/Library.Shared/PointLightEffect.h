#pragma once

#include "ShaderProgram.h"
#include "VertexDeclarations.h"

namespace Library
{
	class PointLightEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(PointLightEffect, ShaderProgram)

		SHADER_VARIABLE_DECLARATION(WorldViewProjection)
		SHADER_VARIABLE_DECLARATION(World)
		SHADER_VARIABLE_DECLARATION(AmbientColor)
		SHADER_VARIABLE_DECLARATION(LightColor)
		SHADER_VARIABLE_DECLARATION(LightPosition)
		SHADER_VARIABLE_DECLARATION(LightRadius)
		SHADER_VARIABLE_DECLARATION(CameraPosition)
		SHADER_VARIABLE_DECLARATION(SpecularColor)
		SHADER_VARIABLE_DECLARATION(SpecularPower)
		SHADER_VARIABLE_DECLARATION(EmissiveIntensity)

    public:
        PointLightEffect();

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