#include "..\Header\Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(const vector<Vertex>& vertices, const vector<Uint32>& indices)
 : vertices(vertices), indices(indices)
{
	Initialize();
}

Mesh::Mesh(const vector<float>& rawVertices, int stride, const vector<Uint32>& indices)
 : vertices(ConvertRawVertices(rawVertices, stride)), indices(indices)
{
	Initialize();
}

void Mesh::Initialize()
{
	string parameterNames[] = { "RenderBackfaces", "DrawElements" };
	parameterCollection = make_unique<ParameterCollection<Parameters, ParametersLength>>(parameterNames);
	parameterCollection->SetParameter(RenderBackfaces, false);
	parameterCollection->SetParameter(DrawElements, true);

	CreateVertexArray();
}

vector<Vertex> Mesh::ConvertRawVertices(const vector<float>& rawVertices, int stride) const
{
	vector<Vertex> vertices;
	for (int i = 0; i < rawVertices.size(); i += stride)
	{
		Vertex vertex;
		int j = 0;

		vertex.position.x = rawVertices[i + j++];
		vertex.position.y = rawVertices[i + j++];
		vertex.position.z = rawVertices[i + j++];

		const int StrideNeededToHaveNormals = 5;
		if (stride > StrideNeededToHaveNormals)
		{
			vertex.normal.x = rawVertices[i + j++];
			vertex.normal.y = rawVertices[i + j++];
			vertex.normal.z = rawVertices[i + j++];
		}

		vertex.textureCoord.x = rawVertices[i + j++];
		vertex.textureCoord.y = rawVertices[i + j++];

		vertices.push_back(vertex);
	}

	return vertices;
}

void Mesh::CreateVertexArray()
{
	Uint32 vertexBufferId, elementBufferId;
	const int Amount = 1;
	glGenVertexArrays(Amount, &vertexArrayId);
	glGenBuffers(Amount, &vertexBufferId);
	glGenBuffers(Amount, &elementBufferId);

	BindVertexArray();

	SendVertices(vertexBufferId);
	SendIndices(elementBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	glDeleteBuffers(Amount, &elementBufferId);
	glDeleteBuffers(Amount, &vertexBufferId);
}

void Mesh::SendVertices(Uint32 vertexBufferId)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

	SendPositionAttribute();
	SendNormalAttribute();
	SendTextureAttribute();
}

void Mesh::SendPositionAttribute()
{
	VertexAttribute positionAttribute;
	positionAttribute.location = 0;
	positionAttribute.size = 3;
	positionAttribute.offset = 0;

	SendVertexAttribute(positionAttribute);
}

void Mesh::SendNormalAttribute()
{
	VertexAttribute normalAttribute;
	normalAttribute.location = 1;
	normalAttribute.size = 3;
	normalAttribute.offset = offsetof(Vertex, normal);

	SendVertexAttribute(normalAttribute);
}

void Mesh::SendTextureAttribute()
{
	VertexAttribute textureAttribute;
	textureAttribute.location = 2;
	textureAttribute.size = 2;
	textureAttribute.offset = offsetof(Vertex, textureCoord);

	SendVertexAttribute(textureAttribute);
}

void Mesh::SendVertexAttribute(const VertexAttribute& attribute)
{
	glVertexAttribPointer(attribute.location, attribute.size, GL_FLOAT, attribute.normalize, attribute.stride, (void*)attribute.offset);
	glEnableVertexAttribArray(attribute.location);
}

void Mesh::SendIndices(Uint32 elementBufferId)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Uint32), &indices.front(), GL_STATIC_DRAW);
}

void Mesh::BindVertexArray()
{
	glBindVertexArray(vertexArrayId);
}

void Mesh::DrawMesh()
{
	parameterCollection->GetParameter(DrawElements) ? DrawTriangleElements() : DrawTriangleArrays();
}

void Mesh::DrawTriangleElements()
{
	const int Offset = 0;
	glDrawElements(GL_TRIANGLES, GetIndiceCount(), GL_UNSIGNED_INT, Offset);
}

void Mesh::DrawTriangleArrays()
{
	const int First = 0;
	glDrawArrays(GL_TRIANGLES, First, GetVerticeCount());
}

int Mesh::GetIndiceCount() const
{
	return (int)indices.size();
}

int Mesh::GetVerticeCount() const
{
	return (int)vertices.size();
}

void Mesh::AddAssociatedTextureName(const string& textureName)
{
	associatedTextureNames.push_back(textureName);
}

void Mesh::AddAssociatedTextureIndices(const vector<string>& textureNames)
{
	associatedTextureNames.insert(associatedTextureNames.end(), textureNames.begin(), textureNames.end());
}

const vector<string>& Mesh::GetAssociatedTextureNames() const
{
	return associatedTextureNames;
}

ParameterCollection<Mesh::Parameters, Mesh::ParametersLength>* Mesh::GetParameterCollection() const
{
	return parameterCollection.get();
}

json Mesh::GetJson() const
{
	return json();
}

void Mesh::SetFromJson(const json& j)
{

}

Mesh::~Mesh()
{
	const int Amount = 1;
	glDeleteVertexArrays(Amount, &vertexArrayId);
}
