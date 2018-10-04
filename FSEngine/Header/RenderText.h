#pragma once
#include "GameObject.h"
#include "Window.h"

#include <SDL.h>
#include <SDL_ttf.h>

using std::vector;
using std::string;

class RenderText : public GameObject
{
public:
	enum AnchorPosition { Center, TopLeft, TopRight, BottomLeft, BottomRight };

private:
	TTF_Font* font = nullptr;
	Window* window = nullptr;

	string renderText;
	vec2 aspectRatio;

	vec2 pixelScaleFactor = vec2(1, 1);
	vec2 pixelPosition = vec2(0, 0);
	AnchorPosition anchorPosition = Center;
	AnchorPosition alignPosition = Center;

	shared_ptr<Mesh> CreateMeshComponent() const;
	void LoadFont(const string& fontName);
	void CreateTextureComponent(const string& text);

	vec2 CalculateAspectRatio(const vec2& surfaceSize);
	void SetScaleFromWindowSize(const vec2& windowSize);

	void SetPositionFromWindowSize(const vec2& windowSize);
	vec2 GetPixelAnchoredPosition(const vec2& windowSize) const;
	vec2 GetPixelAlignPosition(const vec2& position, const vec2& windowSize) const;
	vec2 GetPixelScale(const vec2& windowSize) const;

	void SetPixelPositionToTopLeftOrigin();

public:
	RenderText(Window* window);
	~RenderText();

	void Start() override;
	void Update() override;

	void SetText(const string& text);

	void SetPixelScale(const vec2& pixelScaleFactor);
	void SetPixelScale(float pixelScaleFactor);
	void SetPixelPosition(const vec2& pixelPosition);
	void SetScreenAnchorPoint(AnchorPosition anchorPoint);
	void SetTextAlignment(AnchorPosition alignPosition);
};
