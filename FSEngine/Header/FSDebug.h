#pragma once

#include <glm\glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::vec2;

#include <string>
using std::string;
using std::to_string;

namespace FSDebug
{
	void Print(int message, const string& label);
	void Print(float message, const string& label);
	void Print(const mat4& message, const string& label);
	void Print(const vec4& message, const string& label);
	void Print(const vec3& message, const string& label);
	void Print(const vec2& message, const string& label);
	void Print(const string& message, const string& label);

	string GetMatrixString(const mat4& matrix);
	string GetVectorString(const vec4& vector);
	string GetVectorString(const vec3& vector);
	string GetVectorString(const vec2& vector);
}

template <typename T>
constexpr auto printFS(const T& message, const string& label = "")
{
	return FSDebug::Print(message, label);
}

template <typename T>
constexpr auto printcFS(const T& message, const string& label = "")
{
	system("cls");
	return FSDebug::Print(message, label);
}
