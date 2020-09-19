#pragma once
#include <limits>
#include "DrawableGameComponent.h"
#include "SpotLightEffect.h"
#include "SpotLightShadowedEffect.h"
#include "PointLightEffect.h"
#include "DepthPass.h"
#include "ColorHelper.h"
#include "TextureProjector.h"
#include "ProjectingLight.h"
#include "MovableGameObject.h"
#include "ProjectingLight.h"
#include "ColorHelper.h"
#include "DeferredFramebuffer.h"
#include "DeferredEffect.h"
#include "Game.h"

namespace Library
{
	class Light;
	class DirectionalLight;
	class SpotLight;
	class ProxyModel;

	class RenderedMesh final : public DrawableGameComponent, public MovableGameObject
	{
		RTTI_DECLARATIONS(RenderedMesh, DrawableGameComponent)

	public:
		RenderedMesh(Library::Game& game, std::shared_ptr<Camera> camera,
			const std::string& objectFilename, const std::string& textureFilename = "", int meshIndex = 0);
		RenderedMesh(const RenderedMesh&) = delete;
		RenderedMesh& operator=(const RenderedMesh&) = delete;
		RenderedMesh(RenderedMesh&&) = delete;
		RenderedMesh& operator=(RenderedMesh&&) = delete;
		~RenderedMesh();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void DrawShadowed(const Library::GameTime& gameTime);
		virtual void DrawToGBuffer(const Library::GameTime& gameTime);
		void DepthTest(ProjectingLight& lightSource);

		void SetGBuffer(DeferredFramebuffer& gbuffer);
		void SetAlbedo(glm::vec4 newColor);
		void SetDepthBias(float bias);

		void SetAmbientLight(std::shared_ptr<Light> newLight);
		void SetSpotLight(std::shared_ptr<ProjectingLight> newLight);

		float mSpecularPower = 1.0f;
		float mFogStart = 500.0f;
		float mFogRange = 1000.0f;
		float mAmbientIntensity = 0.5f;

	private:
		int mMeshIndex;

		std::string mObjectFilename;
		std::string mTextureFilename;
		GLuint mColorTextureID = 0;

		std::shared_ptr<Light> mAmbientLight;
		std::shared_ptr<ProjectingLight> mSpotlight;

		glm::mat4 mWorldMatrix{glm::mat4(1)};
		glm::vec4 mSpecularColor = Library::ColorHelper::White;

		DeferredFramebuffer* mDeferredBuffer{ nullptr };

		float mShadowDepthBias = 0.001f;

		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;

		glm::vec4 mFogColor = Library::ColorHelper::CornflowerBlue;
		glm::vec4 mAlbedo = ColorHelper::White;

		bool mTextured = false;

		GLuint mNormalMap = 0;

		//global values that won't change between models can all be initialized once
		inline static bool ProgramsInitialized{ false };

		inline static GLuint mTrilinearSampler{ 0 };
		inline static GLuint mShadowSampler{ 0 };

		inline static SpotLightEffect* mShaderProgram{ nullptr };
		inline static SpotLightShadowedEffect* mShaderProgramShadowed{ nullptr };
		inline static DepthPass* mShaderProgramDepth{ nullptr };
		inline static DeferredEffect* mDeferredProgram{ nullptr };

	};

}