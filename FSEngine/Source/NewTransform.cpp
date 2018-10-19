#include "../Header/NewTransform.h"

void NewTransform::SetPosition(const vec3& position)
{
	this->position = position;
	CalculateMatrix();
}

const vec3& NewTransform::GetPosition() const
{
	return position;
}

void NewTransform::SetScale(const vec3& scale)
{
	this->scale = scale;
	CalculateMatrix();
}

const vec3& NewTransform::GetScale() const
{
	return scale;
}

void NewTransform::SetOrientation(float angle, const vec3& axis)
{
	this->orientation = glm::angleAxis(angle, axis);
	CalculateMatrix();
}

const quat& NewTransform::GetOrientation() const
{
	return orientation;
}

void NewTransform::CalculateMatrix()
{
	matrix = glm::translate(FSMath::IdentityMatrix, position) * glm::toMat4(orientation) * glm::scale(FSMath::IdentityMatrix, scale);
}

void NewTransform::SetMatrix(const mat4& matrix)
{
	this->matrix = matrix;

	vec3 unusedSkew; vec4 unusedPerspective;
	glm::decompose(this->matrix, scale, orientation, position, unusedSkew, unusedPerspective);
}

const mat4& NewTransform::GetMatrix() const
{
	return matrix;
}

json NewTransform::GetJson() const
{
	return json();
}

void NewTransform::SetFromJson(const json& j)
{

}
