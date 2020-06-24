#include "pch.h"
#include "Light.h"

using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(Light)

	Light::Light(Game& game) :
		GameComponent(game)
	{
	}

	const vec4& Light::Color() const
	{
		return mColor;
	}

	void Light::SetColor(float r, float g, float b, float a)
	{
		SetColor(vec4(r, g, b, a));
	}

	void Light::SetColor(const vec4& color)
	{
		mColor = color;
	}
}