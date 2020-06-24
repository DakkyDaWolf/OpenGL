#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <GL/glcorearb.h>
#include "DrawableGameComponent.h"
#include "BasicEffect.h"
#include "VectorHelper.h"

namespace Library
{
	class Game;
	class Camera;

	class Grid final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Grid, DrawableGameComponent)

	public:
		Grid(Game& game, std::shared_ptr<Camera> camera, GLuint size = DefaultSize, GLuint scale = DefaultScale, const glm::vec4& color = DefaultColor);
		Grid(const Grid&) = delete;
		Grid& operator=(const Grid&) = delete;
		Grid(Grid&&) = delete;
		Grid& operator=(Grid&&) = delete;
		~Grid();

		const glm::vec3& Position() const;
		const glm::vec4& Color() const;
		const GLuint Size() const;
		const GLuint Scale() const;

		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z);
		void SetColor(const glm::vec4& color);
		void SetSize(GLuint size);
		void ApplyScale(GLuint scale);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		void InitializeGrid();

		inline static const GLuint DefaultSize = 16;
		inline static const GLuint DefaultScale = 16;
		inline static const glm::vec4 DefaultColor = glm::vec4(0.961f, 0.871f, 0.702f, 1.0f);

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mColor;
		glm::vec3 mPosition = Vector3Helper::Zero;
		BasicEffect mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mSize;
		GLuint mScale;
		std::uint32_t mVertexCount = 0;
	};
}