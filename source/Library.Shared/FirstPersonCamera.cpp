#include "pch.h"
#include "FirstPersonCamera.h"
#include "VectorHelper.h"
#include "glm/gtx/transform.hpp"
#include "Game.h"

using namespace glm;

namespace Library
{
    RTTI_DEFINITIONS(FirstPersonCamera)

    FirstPersonCamera::FirstPersonCamera(Game& game) :
		PerspectiveCamera(game)
    {
    }

    FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		PerspectiveCamera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance)
    {
    }

    float& FirstPersonCamera:: MouseSensitivity()
    {
        return mMouseSensitivity;
    }

    float& FirstPersonCamera::RotationRate()
    {
        return mRotationRate;
    }

    float& FirstPersonCamera::MovementRate()
    {
        return mMovementRate;
    }

    void FirstPersonCamera::Initialize()
    {
        PerspectiveCamera::Initialize();
		glfwGetCursorPos(mGame->Window(), &mLastCursorX, &mLastCursorY);
    }

    void FirstPersonCamera::Update(const GameTime& gameTime)
    {
		bool positionChanged = false;
		vec3 movementAmount = Vector3Helper::Zero;
		if (glfwGetKey(mGame->Window(), GLFW_KEY_W))
		{
			movementAmount.y = 1.0f;
			positionChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_S))
		{
			movementAmount.y = -1.0f;
			positionChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_A))
		{
			movementAmount.x = -1.0f;
			positionChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_D))
		{
			movementAmount.x = 1.0f;
			positionChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_Q))
		{
			movementAmount.z = -1.0f;
			positionChanged = true;
		}
		if (glfwGetKey(mGame->Window(), GLFW_KEY_E))
		{
			movementAmount.z = 1.0f;
			positionChanged = true;
		}

		vec2 rotationAmount = Vector2Helper::Zero;		
		double x, y;
		glfwGetCursorPos(mGame->Window(), &x, &y);		

		if (glfwGetMouseButton(mGame->Window(), GLFW_MOUSE_BUTTON_LEFT))
		{
			rotationAmount.x = static_cast<float>(mLastCursorX - x) * mMouseSensitivity;
			rotationAmount.y = static_cast<float>(mLastCursorY - y) * mMouseSensitivity;
			positionChanged = true;
		}

		mLastCursorX = x;
		mLastCursorY = y;

		if (positionChanged)
		{
			float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
			vec2 rotationVector = rotationAmount * mRotationRate * elapsedTime;

			mat4 yawMatrix = rotate(rotationVector.x, Vector3Helper::Up);
			ApplyRotation(yawMatrix);
			mat4 pitchMatrix = rotate(rotationVector.y, cross(Vector3Helper::Up, vec3(Direction())));
			ApplyRotation(pitchMatrix);

			vec3 movement = movementAmount * mMovementRate * elapsedTime;

			vec3 strafe = mRight * movement.x;
			mPosition += strafe;

			vec3 forward = mForward * movement.y;
			mPosition += forward;

			vec3 up = vec3(0.f, 1.f, 0.f) * movement.z;
			mPosition += up;
		}

        Camera::Update(gameTime);
    }
}
