#include "../Header/ClickLabel.h"

void ClickLabel::Start()
{
	AddComponent(make_shared<QuadMesh>(), "Mesh");
	AddComponent(make_shared<Transform>());
	
	auto shading = AddComponent(make_shared<FontTexture>(), "Shading");
	shading->LoadFont("consola.ttf");
	shading->GetParameterCollection()->SetParameter(Shading::CalculateLighting, false);
	shading->GetParameterCollection()->SetParameter(Shading::EnableDepthTest, false);

	SetSerializable(false);
	GetParameterCollection()->SetParameter(DoLateDraw, true);
	GetParameterCollection()->SetParameter(DoLateUpdate, true);
}

void ClickLabel::InitializeClickLabel(const string& gameObjectName)
{
	transform = GetComponent<Transform>();
	camera = gameObjectContainer->GetGameObjectAs<Camera>("Camera");

	GetComponent<FontTexture>("Shading")->GenerateFontTexture(gameObjectName);
}

void ClickLabel::Update()
{
	transform->SetOrientation(GetCameraLookAtOrientation());

	if (systems->input->IsButtonPressed(SDL_BUTTON_LEFT) && CursorIntersectsQuad())
		printFS(GetName());
}

quat ClickLabel::GetCameraLookAtOrientation() const
{
	vec3 cameraForward = camera->GetForward();
	vec3 lookDirection(cameraForward.x, -cameraForward.y, -cameraForward.z);

	return FSMath::LookAt(lookDirection, camera->GetUp());
}

// https://stackoverflow.com/questions/21114796/3d-ray-quad-intersection-test-in-java
bool ClickLabel::CursorIntersectsQuad() const
{
	corners corners = GetQuadCorners();
	vec3 cursorPlaneIntersectPosition = GetCursorPlaneIntersectPosition();

	vec2 projected2DIntersect = GetProjected2DIntersect(cursorPlaneIntersectPosition, corners);
	return Projected2DIntersectWithinCorners(projected2DIntersect, corners);
}

ClickLabel::corners ClickLabel::GetQuadCorners() const
{
	vec3 horizontalOffset = transform->GetRight() * (transform->GetScale().x / 2);
	vec3 verticalOffset = transform->GetUp() * (transform->GetScale().y / 2);

	corners corners;
	corners.topLeft = transform->GetPosition() - horizontalOffset + verticalOffset;
	corners.topRight = transform->GetPosition() + horizontalOffset + verticalOffset;
	corners.bottomLeft = transform->GetPosition() - horizontalOffset - verticalOffset;

	return corners;
}

vec3 ClickLabel::GetCursorPlaneIntersectPosition() const
{
	ray cursorRay = camera->GetCursorRay();
	plane plane(transform->GetPosition(), transform->GetForward());
	float intersectDistance = FSMath::RayIntersectPlaneDistance(plane, cursorRay);

	return cursorRay.origin + (cursorRay.direction * intersectDistance);
}

vec2 ClickLabel::GetProjected2DIntersect(const vec3& cursorPlaneIntersectPosition, const corners& corners) const
{
	vec2 projected2DPosition;
	projected2DPosition.x = glm::dot(cursorPlaneIntersectPosition - corners.topLeft, corners.topRight - corners.topLeft);
	projected2DPosition.y = glm::dot(cursorPlaneIntersectPosition - corners.topLeft, corners.bottomLeft - corners.topLeft);

	return projected2DPosition;
}

bool ClickLabel::Projected2DIntersectWithinCorners(const vec2& projected2DIntersect, const corners& corners) const
{
	float quadCornersHorizontal2D = glm::dot(corners.topRight - corners.topLeft, corners.topRight - corners.topLeft);
	float quadCornersVertical2D = glm::dot(corners.bottomLeft - corners.topLeft, corners.bottomLeft - corners.topLeft);

	bool withinX = projected2DIntersect.x >= 0 && projected2DIntersect.x <= quadCornersHorizontal2D;
	bool withinY = projected2DIntersect.y >= 0 && projected2DIntersect.y <= quadCornersVertical2D;

	return withinX && withinY;
}

string ClickLabel::GetGameObjectType() const
{
	return "ClickLabel";
}
