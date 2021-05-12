#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <DirectXMath.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Texture.h"

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT3 n;
};

class Mesh
{
public:
	Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::vector<Vertex>& vertices, std::vector<signed int>& indices);
	void DrawMesh(ID3D11DeviceContext* pContext) const;
	static Mesh ProcessMeshData(aiMesh* pMesh, const aiScene* pScene, ID3D11DeviceContext* pContext, ID3D11Device* pDevice,
		const aiMaterial* const* pMaterials);
	static aiString GetTexturePath();
	static Texture GetTex();
private:
	std::unique_ptr<VertexBuffer<Vertex>> vb;
	std::unique_ptr<IndexBuffer> ib;
};
