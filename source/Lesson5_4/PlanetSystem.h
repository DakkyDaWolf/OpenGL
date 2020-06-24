#pragma once

#include <limits>
#include "DrawableGameComponent.h"
#include "PointLightEffect.h"
#include "ColorHelper.h"
#include "Planet.h"

namespace Library
{
	class Light;
	class PointLight;
	class ProxyModel;
}

namespace Rendering
{
	class PlanetSystem final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(PlanetSystem, DrawableGameComponent)

	public:
		PlanetSystem(Library::Game& game, std::shared_ptr<Library::Camera> camera);
		PlanetSystem(const PlanetSystem&) = delete;
		PlanetSystem& operator=(const PlanetSystem&) = delete;
		PlanetSystem(PlanetSystem&&) = delete;
		PlanetSystem& operator=(PlanetSystem&&) = delete;
		~PlanetSystem();

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void InitPlanets();
		void UpdateAmbientLight(const Library::GameTime& gameTime);
		void UpdatePointLight(const Library::GameTime& gameTime);
		void UpdateSpecularLight(const Library::GameTime& gameTime);
		void UpdateTimeScale(const Library::GameTime& gameTime);

		inline static const float LightModulationRate = std::numeric_limits<unsigned char>::max();
		inline static const float LightMovementRate = 10.0f;
		inline static const float RadiusModulationRate = std::numeric_limits<unsigned char>::max();

		std::vector<std::shared_ptr<Planet>> mPlanets;

		glm::mat4 mWorldMatrix{ 1 };
		glm::vec4 mSpecularColor = Library::ColorHelper::White;
		PointLightEffect mShaderProgram;
		GLuint mVertexArrayObject = 0;
		GLuint mVertexBuffer = 0;
		GLuint mIndexBuffer = 0;
		std::size_t mIndexCount = 0;
		GLuint mColorTexture = 0;
		std::unique_ptr<Library::Light> mAmbientLight;
		std::unique_ptr<Library::PointLight> mPointLight;
		float mSpecularPower = 25.0f;
		float mTimeScale = 1.f;
		float mTotalTime = 0.f;
		float mPausedTimescale = 1.f;
		bool mPausePressed = false;

		bool mIsFollowingPlanet = false;
		float mFollowDistance = 20.f;
		size_t mFollowedPlanet = 0;
	};
}
