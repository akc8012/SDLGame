#pragma once
#include "FileSystem.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Timer.h"
#include "GameObject.h"
#include "CubePrimitive.h"
#include "RenderText.h"
#include "Model.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

#include <string>

class Engine
{
private:
	bool running = false;
	float lastFrameTime = 0.0f;

	FileSystem* fileSystem = nullptr;
	Window* window = nullptr;
	ShaderProgram* shaderProgram = nullptr;
	Input* input = nullptr;
	Renderer* renderer = nullptr;

	GameObject* cubeFace = nullptr;
	GameObject* cubeBrick = nullptr;
	RenderText* renderText = nullptr;
	GameObject* model = nullptr;

	void InitSDL();
	void InitOpenGl();
	void InitGlew();

	void ToggleSwapInterval();
	void SetSwapInterval(int interval);

	float CalculateDeltaTime();
	void PollEvents();
	void Update(float deltaTime);
	void Draw(float deltaTime);

	void HandleKeyboardEvent(const SDL_KeyboardEvent& keyboardEvent);
	void HandleWindowEvent(const SDL_WindowEvent& windowEvent);

public:
	~Engine();

	bool IsRunning() const;
	bool Init();

	void GameLoop();
};
