#pragma once
#include "GameObject.h"

#include <SDL.h>
#include <SDL_ttf.h>

class TextQuad : public GameObject
{
private:
	TTF_Font* font = nullptr;
	Texture* texture = nullptr;
	std::string renderText;
	float textAspect;

	void LoadFont(const char* fontName);
	void CreateTexture(const char* text);
	void CreateRenderComponent();

	void SetText(std::string text);

public:
	TextQuad(FileSystem* fileSystem);
	~TextQuad();

	void Update(Uint32 deltaTime);
};
