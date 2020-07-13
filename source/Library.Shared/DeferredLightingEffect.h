#pragma once
#include "ShaderProgram.h"

namespace Library
{
	struct VertexPositionTexture;

	class DeferredLightingEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(DeferredLightingEffect, ShaderProgram)

		SHADER_VARIABLE_DECLARATION(ScreenspaceProjection)
		SHADER_VARIABLE_DECLARATION(AmbientColor)
		SHADER_VARIABLE_DECLARATION(LightPosition)
		SHADER_VARIABLE_DECLARATION(LightColor)
		SHADER_VARIABLE_DECLARATION(LightFalloffRange)
		SHADER_VARIABLE_DECLARATION(LightLookDirection)
		SHADER_VARIABLE_DECLARATION(LightInnerAngle)
		SHADER_VARIABLE_DECLARATION(LightOuterAngle)
		SHADER_VARIABLE_DECLARATION(CameraPosition)
		SHADER_VARIABLE_DECLARATION(FogColor)
		SHADER_VARIABLE_DECLARATION(FogStart)
		SHADER_VARIABLE_DECLARATION(FogRange)

	public:
		DeferredLightingEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1
		};
	};
}