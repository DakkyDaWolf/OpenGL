#pragma once
#include "ShaderProgram.h"

namespace Library
{
	struct VertexPosition;

	class DeferredPointLightEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(DeferredPointLightEffect, ShaderProgram)

			SHADER_VARIABLE_DECLARATION(WorldViewProjection)
			SHADER_VARIABLE_DECLARATION(CameraPosition)

			SHADER_VARIABLE_DECLARATION(LightColor)
			SHADER_VARIABLE_DECLARATION(LightPosition)
			SHADER_VARIABLE_DECLARATION(LightRadius)

			SHADER_VARIABLE_DECLARATION(ScreenDimensions)

	public:
		DeferredPointLightEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum VertexAttribute
		{
			Position = 0
		};
	};
}