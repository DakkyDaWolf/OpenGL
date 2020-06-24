#pragma once

#include "PointLight.h"
#include "VectorHelper.h"

namespace Library
{
	class SpotLight : public PointLight
	{
		RTTI_DECLARATIONS(SpotLight, PointLight)

	public:
		SpotLight(Game& game);
		SpotLight(const SpotLight&) = default;
		SpotLight& operator=(const SpotLight&) = default;
		SpotLight(SpotLight&&) = default;
		SpotLight& operator=(SpotLight&&) = default;
		virtual ~SpotLight() = default;
	
		const glm::vec3& Direction() const;
		const glm::vec3& Up() const;
		const glm::vec3& Right() const;

		float InnerAngle() const;
		void SetInnerAngle(float value);
		
		float OuterAngle() const;
		void SetOuterAngle(float value);

        void ApplyRotation(const glm::mat4& transform);

		inline static const float DefaultInnerAngle = 0.75f;
		inline static const float DefaultOuterAngle = 0.25f;

	protected:
		glm::vec3 mDirection = Vector3Helper::Forward;
		glm::vec3 mUp = Vector3Helper::Up;
		glm::vec3 mRight = Vector3Helper::Right;
		float mInnerAngle = DefaultInnerAngle;
		float mOuterAngle = DefaultOuterAngle;
	};
}