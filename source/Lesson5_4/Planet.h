#pragma once
#include "DrawableGameComponent.h"

namespace Rendering
{

	class Planet
	{
	public:
		Planet() = default;
		explicit Planet(std::string textureFile, Planet* orbitedPlanet = nullptr);
		Planet(const Planet&) = default;
		Planet(Planet&&) noexcept = default;
		Planet& operator=(const Planet&) = default;
		Planet& operator=(Planet&&) noexcept = default;
		~Planet() = default;

		GLuint Texture() const;

		void SetEmissiveIntensity(float emissiveIntensity);
		float EmissiveIntensity() const;

		void SetRotationAxis(glm::vec3 rotationAxis);
		void SetRotationPeriod(float rotationPeriod);
		void SetScale(float scale);

		void AddMoon(Planet& newMoon);
		void RemoveMoon(Planet& moonToRemove);
		Planet* OrbitedPlanet() const;
		void SetOrbitDistance(float orbitDistance);
		void SetOrbitAxis(glm::vec3 orbitAxis);
		void SetOrbitPeriod(float orbitPeriod);

		/// <summary>
		/// Handles opening the color texture in SOIL
		/// </summary>
		void Initialize();

		/// <summary>
		/// Updates the position of this
		/// </summary>
		/// <param name="elapsedTime">Time (seconds) elapsed since start</param>
		/// <param name="translation">Parent transformation to apply to this when calculating position</param>
		void Update(float elapsedTime, glm::vec3 translation);

		/// <summary>
		/// Returns the world transform of this planet
		/// </summary>
		/// <returns>mCurrentTransform</returns>
		glm::mat4 Transform();

		inline const static float EarthDay{ 1.f };
		inline const static float EarthYear{ 365.f * EarthDay };
		inline const static float EarthScale{ 0.125f };
		inline const static float AstronomicalUnit{ 0.5 * 14.96f };

	private:
		std::string mTextureFile;
		GLuint mTextureID;

		float mEmissiveIntensity{ 0.f };

		glm::vec3 mRotationAxis{ glm::vec3(0,1,0) };
		float mRotationPeriod{ 0.f };
		float mScale{ 1.f };

		std::vector<Planet*> mMoons;
		Planet* mOrbitedPlanet{ nullptr };
		float mOrbitDistance{ 0.f };
		glm::vec3 mOrbitAxis{ glm::vec3(0,1,0) };
		float mOrbitPeriod{ 0.f };

		glm::mat4 mCurrentTransform{ glm::mat4(1) };

	};

}