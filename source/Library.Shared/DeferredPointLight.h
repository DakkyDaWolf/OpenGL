#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "Light.h"
#include "DrawableGameComponent.h"
#include "ColorHelper.h"
#include "DeferredFramebuffer.h"
#include "DeferredPointLightEffect.h"
#include "VectorHelper.h"

namespace Library
{
	class DeferredPointLight : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(DeferredPointLight, DrawableGameComponent)

	public:
		DeferredPointLight(Game& game, std::shared_ptr<Camera>& camera);
		DeferredPointLight(const DeferredPointLight&) = default;
		DeferredPointLight& operator=(const DeferredPointLight&) = default;
		DeferredPointLight(DeferredPointLight&&) = default;
		DeferredPointLight& operator=(DeferredPointLight&&) = default;
		virtual ~DeferredPointLight() = default;

		const glm::mat4& Transform();

		void SetPosition(const glm::vec3& position);
		const glm::vec3& Position() const;

		void SetRadius(float radius);
		float Radius() const;
		
		float Intensity() const;
		void SetIntensity(float intensity);

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		void SetColor(glm::vec4 color);
		
		static void SetGBuffer(std::shared_ptr<DeferredFramebuffer> gBuffer);

		float radiusMult = 1.f;

	protected:
		inline static std::shared_ptr<DeferredFramebuffer> mGBuffer{ nullptr };
		inline static DeferredPointLightEffect* mShaderProgram{nullptr};

		glm::vec3 mColor = ColorHelper::White;
		float mIntensity = 1.f;

		float mRadius{ 1.f };
		glm::vec3 mPosition{ Vector3Helper::Zero };
		glm::mat4 mTransform{ glm::mat4(1) };
		glm::mat4 mWorldMatrix{ 1 };

		bool mTransformDirty = true;

		glm::vec3 mTargetColor;
		glm::vec3 mStartColor;
		float mColorFadeAlpha;
		float mColorFadeRate;

		inline static GLuint mTextureSampler{ 0 };
		inline static GLuint mVAO{ 0 };
		inline static GLuint mVertexBuffer{ 0 };
		inline static size_t mIndexCount{ 0 };
		inline static GLuint mIndexBuffer{ 0 };
		inline static bool Initialized{ false };
	};
}