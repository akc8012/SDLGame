#pragma once
#include "Component.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\matrix_decompose.hpp>
#include <glm\gtx\quaternion.hpp>
using namespace glm;

#include <string>
using std::string;

class TransformComponent : public Component
{
private:
	struct MatrixValues
	{
		vec3 scale;
		quat rotation;
		vec3 translation;
		vec3 skew;
		vec4 perspective;
	};

	mat4 transform = mat4(1.0f);

	MatrixValues DecomposeTransformMatrix() const;

public:
	static const ComponentType ComponentTypeId = Transform;

	static string GetFormattedMatrixString(const mat4& matrix);
	static string GetFormattedVectorString(const vec3& vector);
	static string GetFormattedVectorString(const vec2& vector);

	mat4 GetMatrix() const;
	mat3 CalculateNormalMatrix() const;

	vec3 GetScale() const;
	quat GetRotation() const;
	vec3 GetEulerAngles() const;
	vec3 GetPosition() const;

	void SetMatrix(const mat4& matrix);
	void Scale(const vec2& scaleVector);
	void Scale(const vec3& scaleVector);
	void Scale(float scaleFactor);
	void Rotate(float angle, const vec3& axis);
	void Translate(const vec2& translation);
	void Translate(const vec3& translation);

	void SetScale(const vec2& scaleVector);
	void SetScale(float scaleFactor);
	void SetScale(const vec3& scaleVector);
	void SetRotation(float angle, const vec3& axis);
	void SetPosition(const vec2& position);
	void SetPosition(const vec3& position);

	void LookAt(const vec3& position, const vec3& forwardVector, const vec3& upVector);
};
