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
		void DepthTest(ProjectingLight& lightSource);

		void SetAlbedo(glm::vec4 newColor);
		void SetDepthBias(float bias);

		void SetAmbientLight(std::shared_ptr<Light> newLight);
		void SetSpotLight(std::shared_ptr<ProjectingLight> newLight);

		float mSpecularPower = 25.0f;
		float mFogStart = 80.0f;
		float mFogRange = 100.0f;
		float mAmbientIntensity = 0.5f;

	private:
		int mMeshIndex;

		std::string mObjectFilename;
		std::string mTextureFilename;
		GLuint mColorTextureID = 0;

		std::shared_ptr<Light> mAmbientLight;
		std::shared_ptr<ProjectingLight> mSpotlight;

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::White;
		
		SpotLightEffect mShaderProgram;
		SpotLightShadowedEffect mShaderProgramShadowed;
		DepthPass mShaderProgramDepth;

		float mShadowDepthBias = 0.001f;

		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;

		glm::vec4 mFogColor = Library::ColorHelper::CornflowerBlue;
		glm::vec4 mAlbedo = ColorHelper::White;

		bool mTextured = false;

		GLuint mNormalMap = 0;
		GLuint mTrilinearSampler = 0;
		GLuint mShadowSampler = 0;

	};

}