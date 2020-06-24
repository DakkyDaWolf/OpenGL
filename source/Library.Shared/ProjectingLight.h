#pragma once

#include "Light.h"
#include "PerspectiveCamera.h"
#include "VectorHelper.h"

namespace Library
{
	class ProjectingLight : public Light, public PerspectiveCamera
	{
		RTTI_DECLARATIONS(ProjectingLight, Light)

	public:
		ProjectingLight(Game& game);
		ProjectingLight(const ProjectingLight&) = default;
		ProjectingLight& operator=(const ProjectingLight&) = default;
		ProjectingLight(ProjectingLight&&) = default;
		ProjectingLight& operator=(ProjectingLight&&) = default;
		virtual ~ProjectingLight() = default;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		float InnerAngle() const;
		void SetInnerAngle(float value);

		float OuterAngle() const;
		void SetOuterAngle(float value);

		float AttenuationRadius() const;
		void SetAttenuationRadius(float radius);

		inline static const float DefaultInnerAngle = 0.75f;
		inline static const float DefaultOuterAngle = 0.25f;

	protected:

		float mInnerAngle = DefaultInnerAngle;
		float mOuterAngle = DefaultOuterAngle;
		float mAttenuationRadius = 100.f;
	};
}