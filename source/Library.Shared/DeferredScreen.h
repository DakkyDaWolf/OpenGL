#pragma once
#include "ScreenRect.h"
#include "DeferredFramebuffer.h"
#include "ProjectingLight.h"
#include "Light.h"
#include "Camera.h"
#include "DeferredLightingEffect.h"
#include <memory>

namespace Library
{

	class DeferredScreen final : public ScreenRect
	{
		RTTI_DECLARATIONS(DeferredScreen, ScreenRect)

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

		void SetTexture(GLuint textureID) = delete;
		void SetPosition(const glm::vec2 & position) = delete;
		void SetDimensions(const glm::vec2 & dimensions) = delete;

	private:
		glm::mat4 mScreenspaceTransform{ 1 };
		glm::vec2 mPosition{ 0,0 };
		glm::vec2 mDimensions{ 1,1 };
		bool mScreenspaceTransformDirty = true;
		bool mInverseTextureY = false;

		std::shared_ptr<DeferredFramebuffer> mFramebuffer;
		std::shared_ptr<ProjectingLight> mLight;
		std::shared_ptr<Camera> mCamera;
		std::shared_ptr<Light> mAmbientLight;
		DeferredLightingEffect mDeferredShader;

		float mFogStart = 80.0f;
		float mFogRange = 100.0f;
		glm::vec4 mFogColor = Library::ColorHelper::CornflowerBlue;
	};

}