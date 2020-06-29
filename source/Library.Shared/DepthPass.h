#pragma once

#include "ShaderProgram.h"
#include "VertexDeclarations.h"

namespace Library
{
	class Mesh;

	class DepthPass final : public ShaderProgram
	{
		RTTI_DECLARATIONS(DepthPass, ShaderProgram)

		SHADER_VARIABLE_DECLARATION(WorldViewProjection)

	public:
		DepthPass();
		DepthPass(const DepthPass&) = delete;
		DepthPass& operator=(const DepthPass&) = delete;
		DepthPass(DepthPass&&) = delete;
		DepthPass& operator=(DepthPass&&) = delete;
		~DepthPass() = default;

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