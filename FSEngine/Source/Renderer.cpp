#include "../Header/Renderer.h"

Renderer::Renderer(Systems* systems)
{
	this->systems = systems;
}

void Renderer::SetCamera(GameObject* camera)
{
	this->camera = camera;
}

#pragma region StartRender
void Renderer::StartRender()
{
	ClearScreen();
	SetViewMatrices(camera->GetComponent<TransformComponent>("View").get());
}

void Renderer::ClearScreen()
{
	vec3 color = vec3(41, 48, 61);
	color /= 255;

	glClearColor(color.x, color.y, color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewMatrices(TransformComponent* viewTransform)
{
	systems->shaderProgram->SetMatrixUniform("viewMatrix", viewTransform->GetMatrix());
	systems->shaderProgram->SetVectorUniform("viewPosition", viewTransform->GetPosition());
}
#pragma endregion

#pragma region RenderGameObject
void Renderer::RenderGameObject(GameObject* gameObject)
{
	SetTransformMatrices(gameObject->GetComponent<TransformComponent>().get());

	for (auto& mesh : gameObject->GetComponents<MeshComponent>())
	{
		vector<string> textureNames = mesh.second->GetAssociatedTextureNames();
		if (textureNames.size() != 0)
			SetShadingParameters(gameObject->GetComponent<ShadingComponent>(textureNames.front()).get());
		else
			SetShadingParameters(gameObject->GetComponent<ShadingComponent>().get());

		RenderMesh(mesh.second.get());
	}
}

void Renderer::SetTransformMatrices(TransformComponent* transform)
{
	systems->shaderProgram->SetMatrixUniform("modelMatrix", transform->GetMatrix());
	systems->shaderProgram->SetMatrixUniform("normalMatrix", transform->CalculateNormalMatrix());
}

void Renderer::SetShadingParameters(ShadingComponent* shading)
{
	SetDepthTest(shading->GetParameterCollection()->GetParameter(ShadingComponent::EnableDepthTest));
	SetRenderPerspective(shading->GetParameterCollection()->GetParameter(ShadingComponent::RenderPerspective));
	SetBlend(shading->GetParameterCollection()->GetParameter(ShadingComponent::Blend));

	systems->shaderProgram->SetVectorUniform("flatColor", shading->GetFlatColor());
	shading->BindTexture();
}

void Renderer::SetDepthTest(bool enableDepthTest)
{
	if (systems->shaderProgram->GetParameterCollection()->IsInitializedAndEqualTo(ShaderProgram::EnableDepthTest, enableDepthTest))
		return;

	enableDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	systems->shaderProgram->GetParameterCollection()->SetParameter(ShaderProgram::EnableDepthTest, enableDepthTest);
}

void Renderer::SetRenderPerspective(bool renderPerspective)
{
	if (systems->shaderProgram->GetParameterCollection()->IsInitializedAndEqualTo(ShaderProgram::RenderPerspective, renderPerspective))
		return;

	shared_ptr<TransformComponent> projectionTransform = camera->GetComponent<TransformComponent>(renderPerspective ? "Perspective" : "Orthographic");
	systems->shaderProgram->SetMatrixUniform("projectionMatrix", projectionTransform->GetMatrix());

	systems->shaderProgram->SetBoolUniform("renderPerspective", renderPerspective);
	systems->shaderProgram->GetParameterCollection()->SetParameter(ShaderProgram::RenderPerspective, renderPerspective);
}

void Renderer::SetBlend(bool blend)
{
	if (systems->shaderProgram->GetParameterCollection()->IsInitializedAndEqualTo(ShaderProgram::Blend, blend))
		return;

	blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	systems->shaderProgram->GetParameterCollection()->SetParameter(ShaderProgram::Blend, blend);
}

void Renderer::RenderMesh(MeshComponent* mesh)
{
	mesh->BindVertexArray();

	mesh->GetParameterCollection()->GetParameter(MeshComponent::RenderBackfaces) ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
	mesh->DrawMesh();
}
#pragma endregion

#pragma region EndRender
void Renderer::EndRender(Window* window)
{
	window->SwapWindow();
}
#pragma endregion

Renderer::~Renderer()
{

}
