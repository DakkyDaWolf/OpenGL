#pragma once

#include "ShaderProgram.h"

namespace Library
{
	struct VertexPositionTextureNormal;

	class SpotLightEffectAlbedo final : public ShaderProgram
	{
		RTTI_DECLARATIONS(SpotLightEffectAlbedo, ShaderProgram)

		SHADER_VARIABLE_DECLARATION(Albedo)
		SHADER_VARIABLE_DECLARATION(WorldViewProjection)
		SHADER_VARIABLE_DECLARATION(World)
		SHADER_VARIABLE_DECLARATION(AmbientColor)
		SHADER_VARIABLE_DECLARATION(LightColor)
		SHADER_VARIABLE_DECLARATION(LightLookDirection)
		SHADER_VARIABLE_DECLARATION(CameraPosition)
		SHADER_VARIABLE_DECLARATION(SpecularColor)
		SHADER_VARIABLE_DECLARATION(SpecularPower)
		SHADER_VARIABLE_DECLARATION(FogColor)
		SHADER_VARIABLE_DECLARATION(FogStart)
		SHADER_VARIABLE_DECLARATION(FogRange)
		SHADER_VARIABLE_DECLARATION(LightPosition)
		SHADER_VARIABLE_DECLARATION(LightInnerAngle)
		SHADER_VARIABLE_DECLARATION(LightOuterAngle)
		SHADER_VARIABLE_DECLARATION(LightFalloffRange)

	public:
		SpotLightEffectAlbedo();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum VertexAttribute
		{
			Position = 0,
			Normal = 1
		};
	};
}