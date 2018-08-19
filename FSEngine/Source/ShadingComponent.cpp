#include "../Header/ShadingComponent.h"

ShadingComponent::ShadingComponent()
{

}

ShadingComponent::ShadingComponent(const vec3& flatColor)
{
	SetFlatColor(flatColor);
}

ShadingComponent::ShadingComponent(float r, float g, float b)
{
	SetFlatColor(vec3(r, g, b));
}

void ShadingComponent::SetFlatColor(const vec3& flatColor)
{
	this->flatColor = vec4(flatColor, 1.0f);
}

bool ShadingComponent::CanUse() const
{
	return true;
}

void ShadingComponent::Use(ShaderProgram* shaderProgram)
{
	shaderProgram->SetVectorUniform("flatColor", flatColor);

	enableDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	shaderProgram->SetBoolUniform("renderPerspective", renderPerspective);
}

vec4 ShadingComponent::GetFlatColor() const
{
	return flatColor;
}

void ShadingComponent::SetDepthTest(bool enableDepthTest)
{
	this->enableDepthTest = enableDepthTest;
}

bool ShadingComponent::GetDepthTest() const
{
	return enableDepthTest;
}

void ShadingComponent::SetRenderPerspective(bool renderPerspective)
{
	this->renderPerspective = renderPerspective;
}

bool ShadingComponent::GetRenderPerspective() const
{
	return renderPerspective;
}

ShadingComponent::~ShadingComponent()
{

}