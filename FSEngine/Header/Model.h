#pragma once
#include "Vertex.h"
#include "MeshComponent.h"
#include "TextureComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;
using std::unique_ptr;
using Assimp::Importer;

class Model
{
private:
	vector<MeshComponent*> meshComponents;
	vector<TextureComponent*> textureComponents;
	string directory;

	unique_ptr<Importer> LoadModelImporter(const char* filepath);

	void ConvertMeshesOnNode(const aiNode* node, const aiScene* scene);

	MeshComponent* ConvertMeshToComponent(const aiMesh* mesh);
	vector<TextureComponent*> ConvertMaterialToTextures(const aiMaterial* material);

	vector<Vertex> ConvertVertices(const aiMesh* mesh);
	vector<Uint32> ConvertIndices(const aiMesh* mesh);
	vector<TextureComponent*> ConvertTextures(const aiMaterial* material, const aiTextureType& textureType);

public:
	Model(const string& filepath);
	~Model();

	vector<MeshComponent*> GetMeshComponents() const;
	vector<TextureComponent*> GetTextureComponents() const;
};