#pragma once

#include "Light.h"
#include "VectorHelper.h"

namespace Library
{
	class PointLight : public Light
	{
		RTTI_DECLARATIONS(PointLight, Light)

	public:
		PointLight(Game& game);
		PointLight(const PointLight&) = default;
		PointLight& operator=(const PointLight&) = default;
		PointLight(PointLight&&) = default;
		PointLight& operator=(PointLight&&) = default;
		virtual ~PointLight() = default;

		const glm::vec3& Position() const;
		float Radius() const;

		virtual void SetPosition(float x, float y, float z);
        virtual void SetPosition(const glm::vec3& position);
		virtual void SetRadius(float value);

		inline static const float DefaultRadius = 10.0f;

	protected:
		glm::vec3 mPosition = Vector3Helper::Zero;
		float mRadius = DefaultRadius;
	};
}

