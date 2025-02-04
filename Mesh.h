#pragma once
#include "d3dx12.h"
#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#include "Utility.h"
#include <vector>
#include <array>
#include <D3DCompiler.h>

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Mesh
{
public:
	Mesh();
	~Mesh();
	// Vertex and index buffers on CPU side
	ComPtr<ID3DBlob> mCPUVertexBuffer = nullptr;
	ComPtr<ID3DBlob> mCPUIndexBuffer = nullptr;

	// Vertex and index buffers on GPU side
	ComPtr<ID3D12Resource> mGPUVertexBuffer = nullptr;
	ComPtr<ID3D12Resource> mGPUIndexBuffer = nullptr;

	// Vertex and index buffer uploaders
	ComPtr<ID3D12Resource> mVertexBufferUploader = nullptr;
	ComPtr<ID3D12Resource> mIndexBufferUploader = nullptr;

	// Data about buffers.
	UINT mVertexByteStride = 0;
	UINT mVertexBufferByteSize = 0;
	DXGI_FORMAT mIndexFormat = DXGI_FORMAT_R32_UINT;
	UINT mIndexBufferByteSize = 0;
	int  mIndicesCount = 0;

	// Geometry
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	// Material and texture array
	std::vector<Texture*> mTextures;
	Material* mMaterial;
	
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
	void EmptyUploaders();

	// Calculate buffer data for geometry
	void CalculateBufferData(ID3D12Device* d3DDevice, ID3D12GraphicsCommandList* commandList);

	// Calculates buffer data for if being used in dynamic vertex + index buffers
	void CalculateDynamicBufferData();

	void Draw(ID3D12GraphicsCommandList* commandList);
};
