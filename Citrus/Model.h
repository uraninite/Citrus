#pragma once
#include "CBuffer.h"
#include "Mesh.h"
#include "Camera3D.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "imgui\imgui.h"
using namespace DirectX;

struct Transformation
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
};

class Model
{
public:
	Model() = default;
	bool Init(const std::string& filePath, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Render(Camera3D cam);
	XMFLOAT3 SetPos(float x, float y, float z);
	XMFLOAT3 SetRot(float x, float y, float z);
	XMFLOAT3 SetScale(float x, float y, float z);
	XMFLOAT3 GetPos();
	XMFLOAT3 GetRot();
	XMFLOAT3 GetScale();
	aiString GetTexturePath();
	Texture GetTex();
	~Model() = default;
private:
	std::vector<Mesh> meshes;
	bool LoadMesh(const std::string& filePath);
	void LoadNodes(aiNode* pNode, const aiScene* pScene);
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	CBuffer<Transformation>* cbVertexshader = nullptr;
private:
	XMFLOAT3 pos = XMFLOAT3{ 0,0,0 };
	XMFLOAT3 rot = XMFLOAT3{ 0,0,0 };
	XMFLOAT3 scale = XMFLOAT3{ 1,1,1 };
};