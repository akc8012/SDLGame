#pragma once
#include "Component.h"

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

#include <vector>

class RenderComponent : public Component
{
private:
	Uint32 vertexArrayId = NULL;

	std::vector<float> vertices;
	std::vector<Uint32> indices;
	Uint32 triangleCount;

	struct VertexAttribute
	{
		int location;
		int size;
		bool normalize = false;
		int stride;
		int offset;
	};

	void CreateVertexArray(Uint32 stride);
	void SendVertices(Uint32 vertexBufferId, Uint32 stride);
	void SendIndices(Uint32 elementBufferId);

	void SendPositionAttribute(Uint32 stride);
	void SendTextureAttribute(Uint32 stride);
	void SendVertexAttribute(const VertexAttribute& attribute);

public:
	RenderComponent(const std::vector<float>& vertices, const std::vector<Uint32>& indices, Uint32 stride);
	~RenderComponent();

	void BindVertexArray();
	Uint32 GetTriangleCount() const;

	const type_info& GetType() const;
};
