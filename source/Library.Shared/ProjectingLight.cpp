#include "pch.h"
#include "ProjectingLight.h"

namespace Library
{
	RTTI_DEFINITIONS(ProjectingLight)

	ProjectingLight::ProjectingLight(Game& game) :
		PerspectiveCamera(game), Light(game)
	{
	}

	void ProjectingLight::Initialize()
	{
		PerspectiveCamera::Initialize();
		SetAspectRatio(1.f);
	}

	void ProjectingLight::Update(const GameTime& gameTime)
	{
		PerspectiveCamera::Update(gameTime);
		UpdateProjectionMatrix();
	}

	float ProjectingLight::InnerAngle() const
	{
		return mInnerAngle;
	}

	void ProjectingLight::SetInnerAngle(float value)
	{
		mInnerAngle = value;
	}

	float ProjectingLight::OuterAngle() const
	{
		return mOuterAngle;
	}

	void ProjectingLight::SetOuterAngle(float value)
	{
		mOuterAngle = value;
	}

	float ProjectingLight::AttenuationRadius() const
	{
		return mAttenuationRadius;
	}

	void ProjectingLight::SetAttenuationRadius(float radius)
	{
		mAttenuationRadius = radius;
	}

}