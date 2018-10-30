#pragma once
#include "Component.h"
#include "FSMath.h"
#include "ParameterCollection.h"

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

#include <vector>
using std::vector;

class Mesh : public Component
{
public:
	enum Parameters
	{
		RenderBackfaces,
		DrawElements,

		ParametersLength
	};

private:
	struct VertexAttribute
	{
		int location;
		int size;
		bool normalize = false;
		int stride = sizeof(vertex);
		size_t offset;
	};

	unique_ptr<ParameterCollection<Parameters, ParametersLength>> parameterCollection;

	vector<vertex> vertices;
	vector<Uint32> indices;
	vector<string> associatedTextureNames;

	Uint32 vertexArrayId = NULL;

	vector<vertex> ConvertRawVertices(const vector<float>& rawVertices, int stride) const;
	void CreateVertexArray(const vector<vertex>& vertices, const vector<Uint32>& indices);

	void SendVertices(Uint32 vertexBufferId);
	void SendIndices(Uint32 elementBufferId);

	void SendPositionAttribute();
	void SendNormalAttribute();
	void SendTextureAttribute();

	void SendVertexAttribute(const VertexAttribute& attribute);

	void DrawTriangleElements();
	void DrawTriangleArrays();

public:
	static const Types::ComponentType ComponentTypeId = Types::Mesh;
	Types::ComponentType GetComponentTypeId() const override;

	Mesh(const vector<vertex>& vertices, const vector<Uint32>& indices);
	Mesh(const vector<float>& rawVertices, int stride, const vector<Uint32>& indices);
	Mesh();
	~Mesh();

	void BindVertexArray();
	void DrawMesh();

	int GetIndiceCount() const;
	int GetVerticeCount() const;

	void AddAssociatedTextureName(const string& textureName);
	const vector<string>& GetAssociatedTextureNames() const;

	ParameterCollection<Parameters, ParametersLength>* GetParameterCollection() const;

	json GetJson() const override;
	void SetFromJson(const json& j) override;
};
