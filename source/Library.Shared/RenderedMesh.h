#pragma once
#include <limits>
#include "DrawableGameComponent.h"
#include "SpotLightEffect.h"
#include "SpotLightEffectAlbedo.h"
#include "DepthPass.h"
#include "ColorHelper.h"
#include "TextureProjector.h"
#include "ProjectingLight.h"
//#include "MovableGameObject.h"
#include "ProjectingLight.h"
#include "ColorHelper.h"
#include "Game.h"

namespace Library
{
	class Light;
	class DirectionalLight;
	class SpotLight;
	class ProxyModel;

	class RenderedMesh final : public DrawableGameComponent//, public MovableGameObject
	{
		RTTI_DECLARATIONS(RenderedMesh, DrawableGameComponent)

	public:
		RenderedMesh(Library::Game& game, std::shared_ptr<Camera> camera, const std::string& objectFilename, const std::string& textureFilename = "");
		RenderedMesh(const RenderedMesh&) = delete;
		RenderedMesh& operator=(const RenderedMesh&) = delete;
		RenderedMesh(RenderedMesh&&) = delete;
		RenderedMesh& operator=(RenderedMesh&&) = delete;
		~RenderedMesh();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		void DepthTest(ProjectingLight& lightSource);

		void SetAlbedo(glm::vec4 newColor);

		void SetAmbientLight(std::shared_ptr<Light> newLight);
		void SetSpotLight(std::shared_ptr<ProjectingLight> newLight);

		const glm::vec3& Position() const;
		const glm::vec3& Direction() const;
		const glm::vec3& Up() const;
		const glm::vec3& Right() const;

		const glm::mat4& ViewMatrix() const;

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(const glm::vec3& position);

		virtual void Reset();
		virtual void UpdateViewMatrix();
		virtual void ApplyRotation(const glm::mat4& transform);

	protected:

		glm::vec3 mPosition;
		glm::vec3 mDirection;
		glm::vec3 mUp;
		glm::vec3 mRight;

		glm::mat4 mViewMatrix{ 1 };

		bool mViewMatrixDataDirty{ true };

	private:

		std::string mObjectFilename;
		std::string mTextureFilename;
		GLuint mColorTextureID = 0;

		std::shared_ptr<Light> mAmbientLight;
		std::shared_ptr<ProjectingLight> mSpotlight;

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::White;
		SpotLightEffect mShaderProgram;
		SpotLightEffectAlbedo mShaderProgramAlbedo;
		DepthPass mShaderProgramDepth;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mVertexArrayObjectDepth = 0;
		GLuint mVertexBufferDepth = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;

		float mSpecularPower = 25.0f;
		float mFogStart = 20.0f;
		float mFogRange = 40.0f;
		glm::vec4 mFogColor = Library::ColorHelper::CornflowerBlue;
		glm::vec4 mAlbedo = ColorHelper::White;

		bool mTextured = false;

		GLuint mNormalMap = 0;
		GLuint mTrilinearSampler = 0;

	};

}