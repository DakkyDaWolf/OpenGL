#include "pch.h"
#include "Planet.h"
#include "MatrixHelper.h"
#include <glm/gtx/transform.hpp>

namespace Rendering
{

	Planet::Planet(std::string textureFile, Planet* orbitCenter) :
		mTextureFile(textureFile)
	{
		if (orbitCenter) orbitCenter->AddMoon(*this);
	}

	GLuint Planet::Texture() const
	{
		return mTextureID;
	}

	void Planet::SetEmissiveIntensity(float emissiveIntensity)
	{
		mEmissiveIntensity = emissiveIntensity;
	}

	float Planet::EmissiveIntensity() const
	{
		return mEmissiveIntensity;
	}

	void Planet::SetRotationAxis(glm::vec3 rotationAxis)
	{
		mRotationAxis = rotationAxis;
	}

	void Planet::SetRotationPeriod(float rotationPeriod)
	{
		mRotationPeriod = rotationPeriod;
	}

	void Planet::SetScale(float scale)
	{
		mScale = scale;
	}

	void Planet::AddMoon(Planet& newMoon)
	{
		if (newMoon.OrbitedPlanet())
		{
			newMoon.OrbitedPlanet()->RemoveMoon(newMoon);
		}

		mMoons.push_back(&newMoon);
		newMoon.mOrbitedPlanet = this;
	}

	void Planet::RemoveMoon(Planet& moonToRemove)
	{
		for (auto moonIt = mMoons.begin(); moonIt != mMoons.end(); ++moonIt)
		{
			if (*moonIt == &moonToRemove)
			{
				mMoons.erase(moonIt);
				moonToRemove.mOrbitedPlanet = nullptr;
				break;
			}
		}
	}

	Planet* Planet::OrbitedPlanet() const
	{
		return mOrbitedPlanet;
	}

	void Planet::SetOrbitDistance(float orbitDistance)
	{
		mOrbitDistance = orbitDistance;
	}

	void Planet::SetOrbitAxis(glm::vec3 orbitAxis)
	{
		mOrbitAxis = orbitAxis;
	}

	void Planet::SetOrbitPeriod(float orbitPeriod)
	{
		mOrbitPeriod = orbitPeriod;
	}

	void Planet::Initialize()
	{
		// Load the texture
		mTextureID = SOIL_load_OGL_texture(mTextureFile.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		if (mTextureID == 0)
		{
			throw std::runtime_error("SOIL_load_OGL_texture() failed.");
		}
	}

	void Planet::Update(float elapsedTime, glm::vec3 translation)
	{
		mCurrentTransform = glm::mat4
		{
			glm::vec4(1.f,0.f,0.f,0.f),
			glm::vec4(0.f,1.f,0.f,0.f),
			glm::vec4(0.f,0.f,1.f,0.f),
			glm::vec4(0.f,0.f,0.f,1.f)
		};

		//apply planet scale
		mCurrentTransform = glm::scale(glm::vec3(mScale)) * mCurrentTransform;

		//then planet rotation
		if (mRotationPeriod)
		{
			mCurrentTransform = glm::rotate(glm::two_pi<float>() * (elapsedTime / mRotationPeriod), glm::vec3(0.f, 1.f, 0.f)) * mCurrentTransform;
		}

		//then axis tilt
		//which neends an axis of rotation, the cross product of the axis and the up vector
		glm::vec3 rotationTiltAxis = glm::cross(glm::vec3(0.f, 1.f, 0.f), mRotationAxis);
		if (glm::length(rotationTiltAxis) > 0)
		{
			mCurrentTransform = glm::rotate(glm::acos(glm::dot(glm::vec3(0.f, 1.f, 0.f), glm::normalize(mRotationAxis))), rotationTiltAxis) * mCurrentTransform;
		}

		if (mOrbitedPlanet)
		{
			//then orbit rotation
			if (mOrbitPeriod)
			{
				glm::mat4 orbitTransformation = glm::rotate(glm::two_pi<float>() * (elapsedTime / mOrbitPeriod), glm::vec3(0.f, 1.f, 0.f)) * glm::translate(glm::vec3(0.f, 0.f, mOrbitDistance));
				glm::vec3 orbitTranslation(orbitTransformation[3].x, orbitTransformation[3].y, orbitTransformation[3].z);
				mCurrentTransform = glm::translate(orbitTranslation) * mCurrentTransform;
			}
			else
			{
				//then orbit distance from parent
				mCurrentTransform = glm::translate(glm::vec3(0.f, 0.f, mOrbitDistance)) * mCurrentTransform;
			}

			//then orbit tilt
			//which neends an axis of rotation, the cross product of the axis and the up vector
			glm::vec3 orbitTiltAxis = glm::cross(glm::vec3(0.f, 1.f, 0.f), mOrbitAxis);
			if (glm::length(orbitTiltAxis) > 0)
			{
				mCurrentTransform = glm::rotate(glm::acos(glm::dot(glm::vec3(0.f, 1.f, 0.f), mOrbitAxis)), orbitTiltAxis) * mCurrentTransform;
			}

			//then the parent's position
			mCurrentTransform = glm::translate(translation) * mCurrentTransform;

		}

		glm::vec3 myTranslation(mCurrentTransform[3].x, mCurrentTransform[3].y, mCurrentTransform[3].z);

		for (auto& moon : mMoons)
		{
			moon->Update(elapsedTime, myTranslation);
		}
	}

	glm::mat4 Planet::Transform()
	{
		return mCurrentTransform;
	}

}