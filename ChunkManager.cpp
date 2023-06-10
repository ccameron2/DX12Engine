#include "ChunkManager.h"


ChunkManager::ChunkManager(ID3D12GraphicsCommandList* commandList) : mCommandList(commandList)
{
    FastNoiseLite* noise = new FastNoiseLite();
    noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    mNoise = noise;
}

ChunkManager::~ChunkManager()
{
    for (auto& chunk : mSpawnedChunks)
    {
        delete chunk;
    }
    delete mNoise;
}

void ChunkManager::Update(const XMFLOAT3& playerPosition)
{
    // Calculate the current chunk position based on the player's position
    int currentChunkX = static_cast<int>(playerPosition.x) / mSize;
    int currentChunkZ = static_cast<int>(playerPosition.z) / mSize;

    // Iterate over the 3x3 area centered around the current chunk
    for (int xOffset = -1; xOffset <= 1; xOffset++)
    {
        for (int zOffset = -1; zOffset <= 1; zOffset++)
        {
            int chunkX = currentChunkX + xOffset;
            int chunkZ = currentChunkZ + zOffset;

            XMFLOAT3 chunkPosition(chunkX * mSize, 0.0f, chunkZ * mSize);

            // Check if the chunk is already spawned
            if (!IsChunkOverlapping(chunkPosition))
            {
                // Create and spawn the chunk
                TerrainChunk* chunk = CreateChunk(chunkPosition);
                mSpawnedChunks.push_back(chunk);
                Model* newModel = new Model("", mCommandList, chunk->mMesh);
                mSpawnedChunkModels.push_back(newModel);
            }
        }
    }

    // Remove any chunks that are outside the 3x3 area
    mSpawnedChunks.erase(
        std::remove_if(
            mSpawnedChunks.begin(),
            mSpawnedChunks.end(),
            [currentChunkX, currentChunkZ,this](TerrainChunk* chunk)
            {
                return IsChunkOutsideArea(chunk, currentChunkX, currentChunkZ, mSize);
            }),
        mSpawnedChunks.end()
    );
}

void ChunkManager::Draw(ID3D12GraphicsCommandList* commandList)
{
    //// Get reference to current per object constant buffer
    //auto objectCB = FrameResources[CurrentFrameResourceIndex]->mPerObjectConstantBuffer->GetBuffer();

    //// Get size of the per object constant buffer 
    //UINT objCBByteSize = CalculateConstantBufferSize(sizeof(PerObjectConstants));

    //auto matCB = FrameResources[CurrentFrameResourceIndex]->mPerMaterialConstantBuffer->GetBuffer();

    //UINT matCBByteSize = CalculateConstantBufferSize(sizeof(PerMaterialConstants));

    //// Offset to the CBV for this object
    //auto objCBAddress = objectCB->GetGPUVirtualAddress() + mObjConstBufferIndex * objCBByteSize;
    //commandList->SetGraphicsRootConstantBufferView(1, objCBAddress);

    for (auto& chunk : mSpawnedChunkModels)
    {
        chunk->Draw(commandList);
    }
}

bool ChunkManager::IsChunkOverlapping(const XMFLOAT3& position) const
{
    // Check if the new chunk position overlaps with any existing chunks
    for (auto& chunk : mSpawnedChunks)
    {
        float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&position) - XMLoadFloat3(&chunk->mPosition)));
        if (distance < mSize)
        {
            // Chunks overlap
            return true;
        }
    }

    // No overlap
    return false;
}

TerrainChunk* ChunkManager::CreateChunk(const XMFLOAT3& position) const
{
    auto terrainChunk = new TerrainChunk(mCommandList, mNoise, position);
    return terrainChunk;
}

bool ChunkManager::IsChunkOutsideArea(const TerrainChunk* chunk, int currentChunkX, int currentChunkZ, int size)
{
    int chunkX = static_cast<int>(chunk->mPosition.x) / size;
    int chunkZ = static_cast<int>(chunk->mPosition.z) / size;

    return (std::abs(chunkX - currentChunkX) > 1 || std::abs(chunkZ - currentChunkZ) > 1);
}
