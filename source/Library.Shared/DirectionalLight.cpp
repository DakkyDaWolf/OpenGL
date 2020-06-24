#include "pch.h"
#include "DirectionalLight.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(DirectionalLight)

	DirectionalLight::DirectionalLight(Game& game) :
		Light(game)
	{
	}

	const vec3& DirectionalLight::Direction() const
	{
		return mDirection;
	}
	
	const vec3& DirectionalLight::Up() const
	{
		return mUp;
	}

	const vec3& DirectionalLight::Right() const
	{
		return mRight;
	}

    void DirectionalLight::ApplyRotation(const mat4& transform)
    {
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = static_cast<vec3>(normalize(direction));

		vec4 up = transform * vec4(mUp, 0.0f);
		mUp = static_cast<vec3>(normalize(up));

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
    }
}