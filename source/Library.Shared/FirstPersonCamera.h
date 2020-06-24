#pragma once

#include "PerspectiveCamera.h"

namespace Library
{
    class FirstPersonCamera : public PerspectiveCamera
    {
        RTTI_DECLARATIONS(FirstPersonCamera, PerspectiveCamera)

    public:
        FirstPersonCamera(Game& game);
        FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
		FirstPersonCamera(const FirstPersonCamera&) = delete;
		FirstPersonCamera& operator=(const FirstPersonCamera&) = delete;
		FirstPersonCamera(FirstPersonCamera&&) = delete;
		FirstPersonCamera& operator=(FirstPersonCamera&&) = delete;
        virtual ~FirstPersonCamera() = default;

        float& MouseSensitivity();
        float& RotationRate();
        float& MovementRate();
        
        virtual void Initialize() override;
        virtual void Update(const GameTime& gameTime) override;

		inline static const float DefaultRotationRate = glm::radians(1.0f);
		inline static const float DefaultMovementRate = 50.0f;
		inline static const float DefaultMouseSensitivity = 3.0f;
		
    protected:
        float mMouseSensitivity = DefaultMouseSensitivity;
        float mRotationRate = DefaultRotationRate;
        float mMovementRate = DefaultMovementRate;

    private:
		double mLastCursorX = 0.0;
		double mLastCursorY = 0.0;
    };
}