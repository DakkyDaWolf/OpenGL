#include "pch.h"
#include "SpotLight.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(SpotLight)

	SpotLight::SpotLight(Game& game) :
		PointLight(game)
	{
	}

	const vec3& SpotLight::Direction() const
	{
		return mDirection;
	}
	
	const vec3& SpotLight::Up() const
	{
		return mUp;
	}

	const vec3& SpotLight::Right() const
	{
		return mRight;
	}

	float SpotLight::InnerAngle() const
	{
		return mInnerAngle;
	}

	void SpotLight::SetInnerAngle(float value)
	{
		mInnerAngle = value;
	}

	float SpotLight::OuterAngle() const
	{
		return mOuterAngle;
	}

	void SpotLight::SetOuterAngle(float value)
	{
		mOuterAngle = value;
	}

    void SpotLight::ApplyRotation(const mat4& transform)
    {
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = static_cast<vec3>(normalize(direction));

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = static_cast<vec3>(normalize(up));

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
    }
}