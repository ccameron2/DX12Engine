#include "TerrainTile.h"

TerrainChunk::TerrainChunk(ID3D12GraphicsCommandList* commandList, FastNoiseLite* noise, XMFLOAT3 position)
{
    mPosition = position;
    mNoise = noise;
    CreateMeshGeometry(commandList);
}

TerrainChunk::~TerrainChunk()
{
	delete mMesh;
	mTriangles.clear();
	mVertices.clear();
}

void TerrainChunk::CreateMeshGeometry(ID3D12GraphicsCommandList* commandList)
{
    mMesh = new Mesh();
    
    GenerateGrid(mSize,mSpacing,mVertices,mTriangles);
    ApplyNoise(0.35,6,mVertices);

    mMesh->mIndices.resize(mTriangles.size() * 3);

    int index = 0;
    for (auto& triangle : mTriangles)
    {
        mMesh->mIndices[index] = triangle.Point[0];
        mMesh->mIndices[index + 1] = triangle.Point[1];
        mMesh->mIndices[index + 2] = triangle.Point[2];
        index += 3;
    }

    auto normals = CalculateNormals(mVertices, mMesh->mIndices);

    mMesh->mVertices.resize(mVertices.size());

    index = 0;
    for (auto& vertex : mVertices)
    {
        mMesh->mVertices[index].Pos = AddFloat3(vertex,mPosition).Pos;
        mMesh->mVertices[index].Normal = normals[index];
        index++;
    }

    mMesh->CalculateBufferData(D3DDevice.Get(),commandList);
}

void TerrainChunk::GenerateGrid(int size, float spacing, std::vector<XMFLOAT3>& vertices, std::vector<Triangle>& triangles)
{
    // Calculate the number of vertices and triangles
    int numVertices = (size + 1) * (size + 1);
    int numTriangles = size * size * 2;

    // Resize the vectors to hold the vertices and triangles
    vertices.resize(numVertices);
    triangles.resize(numTriangles);

    // Generate the vertices
    for (int row = 0; row <= size; ++row)
    {
        for (int col = 0; col <= size; ++col)
        {
            int index = row * (size + 1) + col;
            vertices[index] = XMFLOAT3(col * spacing, 0.0f, row * spacing);
        }
    }

    // Generate the triangles
    int triangleIndex = 0;
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            int topLeft = row * (size + 1) + col;
            int topRight = topLeft + 1;
            int bottomLeft = (row + 1) * (size + 1) + col;
            int bottomRight = bottomLeft + 1;

            // First triangle
            triangles[triangleIndex].Point[0] = topLeft;
            triangles[triangleIndex].Point[1] = bottomLeft;
            triangles[triangleIndex].Point[2] = topRight;
            ++triangleIndex;

            // Second triangle
            triangles[triangleIndex].Point[0] = topRight;
            triangles[triangleIndex].Point[1] = bottomLeft;
            triangles[triangleIndex].Point[2] = bottomRight;
            ++triangleIndex;
        }
    }
}

void TerrainChunk::ApplyNoise(float frequency, int octaves, std::vector<XMFLOAT3>&vertices)
{
    int index = 0;
    for (auto& vertex : vertices)
    {
        auto position = vertex;// MulFloat3(vertex, { 20,20,20 });
        auto elevationValue = FractalBrownianMotion(mNoise, AddFloat3(position, mPosition).Pos, octaves, frequency);

        elevationValue *= 100;

        vertex.y += elevationValue;

        index++;
    }
}
