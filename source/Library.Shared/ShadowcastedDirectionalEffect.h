#pragma once

#include "ShaderProgram.h"

namespace Library
{
	struct VertexPositionTextureNormalTangentBinormal;

	class ShadowcastedDirectionalEffect final : public ShaderProgram
	{
		RTTI_DECLARATIONS(ShadowcastedDirectionalEffect, ShaderProgram)

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
		ShadowcastedDirectionalEffect();

		virtual void Initialize(GLuint vertexArrayObject) override;

	private:
		enum VertexAttribute
		{
			Position = 0,
			TextureCoordinate = 1,
			Normal = 2,
			Tangent = 3,
			Binormal = 4
		};
	};
}