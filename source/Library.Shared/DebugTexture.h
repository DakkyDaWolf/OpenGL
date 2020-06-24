#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <GL/glcorearb.h>
#include "DrawableGameComponent.h"
#include "FlatTextureEffect.h"
#include "VectorHelper.h"

namespace Library
{
	class Game;
	class Camera;

	class DebugTexture final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(DebugTexture, DrawableGameComponent)

	public:
		DebugTexture(Game& game, float scale = DefaultScale);
		DebugTexture(const DebugTexture&) = delete;
		DebugTexture& operator=(const DebugTexture&) = delete;
		DebugTexture(DebugTexture&&) = delete;
		DebugTexture& operator=(DebugTexture&&) = delete;
		~DebugTexture();

		const float Scale() const;
		void ApplyScale(float scale);

		GLuint TextureID() const;
		void SetTextureID(GLuint textureID);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		void InitializeDebugTexture();

		inline static const float DefaultScale = 0.25f;

		FlatTextureEffect mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mColorTextureID = 0;
		float mScale;
		std::uint32_t mVertexCount = 0;
	};
}