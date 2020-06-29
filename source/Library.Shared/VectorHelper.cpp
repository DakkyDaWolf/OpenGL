#include "pch.h"
#include "VectorHelper.h"

using namespace glm;
using namespace std;

namespace Library
{
	const vec2 Vector2Helper::Zero = vec2(0.0f, 0.0f);
	const vec2 Vector2Helper::One = vec2(1.0f, 1.0f);

	string Vector2Helper::ToString(const vec2& vector)
	{
		stringstream stream;

		stream << "{" << vector.x << ", " << vector.y << "}";

		return stream.str();
	}

	const vec3 Vector3Helper::Zero = vec3(0.0f, 0.0f, 0.0f);
	const vec3 Vector3Helper::One = vec3(1.0f, 1.0f, 1.0f);
	const vec3 Vector3Helper::Forward = vec3(0.0f, 0.0f, -1.0f);
	const vec3 Vector3Helper::Backward = vec3(0.0f, 0.0f, 1.0f);
	const vec3 Vector3Helper::Up = vec3(0.0f, 1.0f, 0.0f);
	const vec3 Vector3Helper::Down = vec3(0.0f, -1.0f, 0.0f);
	const vec3 Vector3Helper::Right = vec3(1.0f, 0.0f, 0.0f);
	const vec3 Vector3Helper::Left = vec3(-1.0f, 0.0f, 0.0f);

	string Vector3Helper::ToString(const vec3& vector)
	{
		stringstream stream;

		stream << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}";

		return stream.str();
	}

	const vec4 Vector4Helper::Zero = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	const vec4 Vector4Helper::One = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const vec4 Vector4Helper::Forward = vec4(Vector3Helper::Forward, 0.0f);
	const vec4 Vector4Helper::Backward = vec4(Vector3Helper::Backward, 0.0f);
	const vec4 Vector4Helper::Right = vec4(Vector3Helper::Right, 0.0f);
	const vec4 Vector4Helper::Left = vec4(Vector3Helper::Left, 0.0f);
	const vec4 Vector4Helper::Up = vec4(Vector3Helper::Up, 0.0f);
	const vec4 Vector4Helper::Down = vec4(Vector3Helper::Down, 0.0f);

	string Vector4Helper::ToString(const vec4& vector)
	{
		stringstream stream;

		stream << "{" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << "}";

		return stream.str();
	}
}
