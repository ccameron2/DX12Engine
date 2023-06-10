#include "Utility.h"
#include <vector>
#include <algorithm>
#include "TerrainTile.h"
#include "Model.h"

class ChunkManager
{
public:
    ChunkManager(ID3D12GraphicsCommandList* commandList);
    ~ChunkManager();

    void Update(const XMFLOAT3& playerPosition);
    void Draw(ID3D12GraphicsCommandList* commandList);
    int mObjConstBufferIndex = 0;
    std::vector<Model*> mSpawnedChunkModels;

private:
    ID3D12GraphicsCommandList* mCommandList;
    FastNoiseLite* mNoise;
    int mSize = 100;
    std::vector<TerrainChunk*> mSpawnedChunks;

    // Utility functions
    bool IsChunkOverlapping(const XMFLOAT3& position) const;
    TerrainChunk* CreateChunk(const XMFLOAT3& position) const;
    bool IsChunkOutsideArea(const TerrainChunk* chunk, int currentChunkX, int currentChunkZ, int size);
};

