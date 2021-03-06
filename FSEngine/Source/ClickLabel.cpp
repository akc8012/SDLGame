#include "../Header/ClickLabel.h"

#pragma region Initialize
void ClickLabel::Start()
{
	AddComponent(make_shared<Transform>());
	
	auto fontTexture = AddComponent(make_shared<FontTexture>(), "FontTextureShading");
	fontTexture->LoadFont("consola.ttf");
	fontTexture->GetParameterCollection()->SetParameter(Shading::CalculateLighting, false);
	fontTexture->GetParameterCollection()->SetParameter(Shading::EnableDepthTest, false);

	auto backingColor = AddComponent(make_shared<Color>(DeselectedColor), "BackingColor");
	backingColor->GetParameterCollection()->SetParameter(Shading::CalculateLighting, false);
	backingColor->GetParameterCollection()->SetParameter(Shading::EnableDepthTest, false);

	AddComponent(make_shared<QuadMesh>(), "FontTextureMesh")->SetShadingName("FontTextureShading");
	AddComponent(make_shared<QuadMesh>(), "BackingMesh")->SetShadingName("BackingColor");

	SetSerializable(false);

	bool editorMode = systems->fileSystem->GetSettingsValue<bool>("EditorMode");
	GetParameterCollection()->SetParameter(DoUpdate, editorMode);
	GetParameterCollection()->SetParameter(DoDraw, editorMode);

	GetParameterCollection()->SetParameter(DoLateDraw, true);
	GetParameterCollection()->SetParameter(DoLateUpdate, true);

	systems->eventSystem->AddListener("EditorModeEnabled", this);
	systems->eventSystem->AddListener("EditorModeDisabled", this);
}

void ClickLabel::InitializeClickLabel(IGameObject* attachedGameObject)
{
	this->attachedGameObject = attachedGameObject;

	transform = GetComponent<Transform>();
	camera = gameObjectContainer->GetGameObjectAs<Camera>("Camera");

	auto fontTexture = GetComponent<FontTexture>("FontTextureShading");
	fontTexture->GenerateFontTexture(attachedGameObject->GetName());

	SetScaleFromSurfaceSize(fontTexture->GetSurfaceSize());
}

void ClickLabel::SetScaleFromSurfaceSize(const vec2& surfaceSize)
{
	vec2 aspectRatio = FSMath::CalculateAspectRatio(surfaceSize);
	transform->SetScale(vec2(aspectRatio.x, aspectRatio.y));

	const float ScaleAdjustment = 0.25f;
	transform->Scale(vec2(ScaleAdjustment, ScaleAdjustment));
}
#pragma endregion

void ClickLabel::ReceiveEvent(const string& key, const json& event)
{
	bool editorMode = key == "EditorModeEnabled";

	GetParameterCollection()->SetParameter(DoUpdate, editorMode);
	GetParameterCollection()->SetParameter(DoDraw, editorMode);
}

void ClickLabel::Update()
{
	transform->SetOrientation(GetCameraLookAtOrientation());
	transform->SetPosition(attachedGameObject->GetComponent<Transform>()->GetPosition());
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

IGameObject* ClickLabel::GetAttachedGameObject() const
{
	return attachedGameObject;
}

void ClickLabel::Select()
{
	GetComponent<Color>("BackingColor")->SetColor(SelectedColor);
}

void ClickLabel::Deselect()
{
	GetComponent<Color>("BackingColor")->SetColor(DeselectedColor);
}

string ClickLabel::GetGameObjectType() const
{
	return "ClickLabel";
}

ClickLabel::~ClickLabel()
{
	systems->eventSystem->RemoveListener(this);
}
