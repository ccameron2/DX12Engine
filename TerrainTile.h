#pragma once
#include "Utility.h"
#include "Mesh.h"
#include "Common.h"
#include "FastNoiseLite.h"

class TerrainChunk
{
public:
	TerrainChunk(ID3D12GraphicsCommandList* commandList, FastNoiseLite* noise, XMFLOAT3 position);
	~TerrainChunk();
	Mesh* mMesh;
	int mSize = 100;
	float mSpacing = 5.0f;
	XMFLOAT3 mPosition;
private:
	void CreateMeshGeometry(ID3D12GraphicsCommandList* commandList);
	void GenerateGrid(int size, float spacing, std::vector<XMFLOAT3>& vertices, std::vector<Triangle>& triangles);
	void ApplyNoise(float frequency, int octaves, std::vector<XMFLOAT3>& vertices);
	FastNoiseLite* mNoise;
	std::vector<Triangle> mTriangles;
	std::vector<XMFLOAT3> mVertices;
};

