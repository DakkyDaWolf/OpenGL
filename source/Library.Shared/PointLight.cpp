#include "pch.h"
#include "PointLight.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(PointLight)

	PointLight::PointLight(Game& game) :
		Light(game)
	{
	}

	const vec3& PointLight::Position() const
	{
		return mPosition;
	}

	float PointLight::Radius() const
	{
		return mRadius;
	}

	void PointLight::SetPosition(float x, float y, float z)
    {
		mPosition = vec3(x, y, z);
    }

    void PointLight::SetPosition(const vec3& position)
    {
        mPosition = position;
    }

	void PointLight::SetRadius(float value)
	{
		mRadius = value;
	}
}