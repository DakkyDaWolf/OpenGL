#pragma once
#include "DrawableGameComponent.h"
#include "DeferredFramebuffer.h"
#include "ProjectingLight.h"
#include "Light.h"
#include "Camera.h"
#include "DeferredLightingEffect.h"
#include <memory>

namespace Library
{

	class DeferredScreen final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(DeferredScreen, DrawableGameComponent)

	public:
		DeferredScreen(Game& game, std::shared_ptr<Camera>& camera, std::shared_ptr<DeferredFramebuffer>& fbo, std::shared_ptr<ProjectingLight>& light);
		DeferredScreen(const DeferredScreen&) = default;
		DeferredScreen& operator=(const DeferredScreen&) = default;
		DeferredScreen(DeferredScreen&&) = default;
		DeferredScreen& operator=(DeferredScreen&&) = default;
		virtual ~DeferredScreen() = default;

		virtual void Initialize() override;
		virtual void Draw(const GameTime & gameTime) override;
		void SetAmbientLight(std::shared_ptr<Light>& light);

	private:
		GLuint mTextureSampler{ 0 };
		GLuint mRectVAO{ 0 };
		GLuint mRectVertexBuffer{ 0 };
		GLuint mIndexCount{ 0 };
		GLuint mIndexBuffer{ 0 };

		std::shared_ptr<DeferredFramebuffer> mFramebuffer;
		std::shared_ptr<ProjectingLight> mLight;
		std::shared_ptr<Camera> mCamera;
		std::shared_ptr<Light> mAmbientLight;
		DeferredLightingEffect mDeferredShader;

		float mFogStart = 150.0f;
		float mFogRange = 500.0f;
		glm::vec4 mFogColor = Library::ColorHelper::CornflowerBlue;
	};

}