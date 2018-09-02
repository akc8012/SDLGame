#pragma once
#include "Component.h"
#include "ShaderProgram.h"
#include "ParameterCollection.h"

#include <glm\glm.hpp>
using namespace glm;

class ShadingComponent : public Component
{
public:
	enum Parameters
	{
		EnableDepthTest,
		RenderPerspective,
		Blend,

		ParametersLength
	};

private:
	unique_ptr<ParameterCollection<Parameters, ParametersLength>> parameterCollection;
	vec4 flatColor = vec4(0);

	void Initialize();

public:
	static const ComponentType ComponentTypeId = Shading;

	ShadingComponent();
	ShadingComponent(const vec3& flatColor);
	ShadingComponent(float r, float g, float b);

	virtual void BindTexture();

	void SetFlatColor(const vec3& flatColor);
	vec4 GetFlatColor() const;

	ParameterCollection<Parameters, ParametersLength>* GetParameterCollection() const;
};
