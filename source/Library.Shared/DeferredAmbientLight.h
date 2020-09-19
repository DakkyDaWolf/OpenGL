#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "Light.h"
#include "DrawableGameComponent.h"
#include "ColorHelper.h"
#include "DeferredFramebuffer.h"
#include "DeferredAmbientLightEffect.h"

namespace Library
{
	class DeferredAmbientLight : public DrawableGameComponent, public Light
	{
		RTTI_DECLARATIONS(DeferredAmbientLight, DrawableGameComponent)

	public:
		DeferredAmbientLight(Game& game, std::shared_ptr<DeferredFramebuffer>& gBuffer);
		DeferredAmbientLight(const DeferredAmbientLight&) = default;
		DeferredAmbientLight& operator=(const DeferredAmbientLight&) = default;
		DeferredAmbientLight(DeferredAmbientLight&&) = default;
		DeferredAmbientLight& operator=(DeferredAmbientLight&&) = default;
		virtual ~DeferredAmbientLight() = default;

		float Intensity() const;
		void SetIntensity(float intensity);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	protected:
		std::shared_ptr<DeferredFramebuffer> mGBuffer = nullptr;
		glm::vec3 mColor = ColorHelper::White;
		float mIntensity = 0.5f;
		DeferredAmbientLightEffect mShaderProgram;

		GLuint mTextureSampler{ 0 };
		GLuint mRectVAO{ 0 };
		GLuint mRectVertexBuffer{ 0 };
		GLuint mIndexCount{ 0 };
		GLuint mIndexBuffer{ 0 };
	};
}