#include "blockinfo.h"
#include "chunk.h"
#include "define.h"
#include "engine.h"
#include "perlin.h"

KRYO_BEGIN_NAMESPACE

Chunk::Chunk(Engine* engine, int x, int z)
    : m_engine(engine), m_isDirty(false), m_posX(x), m_posZ(z), m_blocks(CHUNK_SIZE_WIDTH, CHUNK_SIZE_HEIGHT, CHUNK_SIZE_DEPTH)
{
    m_chunks = m_engine->GetChunkArray();
    PopulateChunk(95);
}

Chunk::Chunk(Chunk &source)
    : m_engine(source.m_engine), m_isDirty(false), m_posX(source.m_posX), m_posZ(source.m_posZ), m_blocks(Array3d<BlockType>(source.m_blocks))
{
    m_chunks = m_engine->GetChunkArray();
}

Chunk::~Chunk() { }

void Chunk::PopulateArrayTest()
{
    // Plancher
    for (size_t i = 0; i < 256; ++i)
        m_blocks.Set(i, BTYPE_GRASS);

    // Arche
    for (size_t i = 0; i < 3; ++i)
    {
        m_blocks.Set(15, 1 + i, 1, BTYPE_DIRT);
        m_blocks.Set(15, 1 + i, 3, BTYPE_DIRT);
    }

    m_blocks.Set(15, 3, 2, BTYPE_DIRT);

    // Marches
    for (size_t i = 0; i < 4; ++i)
        m_blocks.Set(8, 1 + i, 6 + i, BTYPE_DIRT);

    // Mur axe des X
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 4; ++j)
            m_blocks.Set(15, 1 + i, 5 + j, BTYPE_DIRT);

    // Mur axe des Z
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 4; ++j)
            m_blocks.Set(10 + j, 1 + i, 15, BTYPE_DIRT);

    m_isDirty = true;
}

void Chunk::PopulateChunk(int seed)
{
    Perlin perlin(6, 8, 50, seed);
    for (size_t x = 0; x < CHUNK_SIZE_WIDTH; ++x)
    {
        for (size_t z = 0; z < CHUNK_SIZE_DEPTH; ++z)
        {
            float val = perlin.Get((float)(m_posX * CHUNK_SIZE_WIDTH + x) / 2000.f, (float)(m_posZ * CHUNK_SIZE_DEPTH + z) / 2000.f);
            int maxHeight = (int)val + (CHUNK_SIZE_HEIGHT / 2);
            for (size_t y = 0; y < maxHeight; ++y)
            {
                if (y < maxHeight - 4)
                    m_blocks.Set(x, y, z, BTYPE_STONE);
                else if (y < maxHeight - 1)
                    m_blocks.Set(x, y, z, BTYPE_DIRT);
                else
                    m_blocks.Set(x, y, z, BTYPE_GRASS);
            }
        }
    }

    m_isDirty = true;
}

void Chunk::Remove(int idx)
{
    Set(idx, BTYPE_AIR);
}

void Chunk::Remove(int nx, int ny, int nz)
{
    Set(nx, ny, nz, BTYPE_AIR);
}

void Chunk::Set(int idx, BlockType type)
{
    m_isDirty = true;
    m_blocks.Set(idx, type);
}

void Chunk::Set(int nx, int ny, int nz, BlockType type)
{
    m_isDirty = true;
    m_blocks.Set(nx, ny, nz, type);
}

void Chunk::Render() const
{
    m_chunkMesh.Render();
}

bool Chunk::IsDirty() const
{
    return m_isDirty;
}

void Chunk::Invalidate()
{
    m_isDirty = true;
}

BlockType Chunk::Get(int idx) const
{
    return m_blocks.Get(idx);
}

BlockType Chunk::Get(int x, int y, int z) const
{
    return m_blocks.Get(x, y, z);
}

BlockType Chunk::Get_s(int x, int y, int z) const
{
    if (   x < 0 || x >= CHUNK_SIZE_WIDTH
        || y < 0 || y >= CHUNK_SIZE_HEIGHT
        || z < 0 || z >= CHUNK_SIZE_DEPTH)
        return BTYPE_AIR;

    return m_blocks.Get(x, y, z);
}

void Chunk::AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, BlockInfo* bi, float absX, float absY, int chunkX, int chunkY, int x, int y, int z)
{
    float rX = x + absX,
          rZ = z + absY;

    const BlockInfo::BlockUV* m = bi->GetUVMap();

    // TODO: vérifier pour la nature des blocs environnants (ex.: IsSolid())

    // Front
    if (m_engine->GetBlock_s(chunkX, chunkY, x, y, z + 1) == BTYPE_AIR)
    {
        vd[count++] = ChunkMesh::VertexData(rX, y, rZ + 1.f, .8f, .8f, .8f, m->fu, m->fv);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y, rZ + 1.f, .8f, .8f, .8f, m->fu + m->fw, m->fv);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y + 1.f, rZ + 1.f, .8f, .8f, .8f, m->fu + m->fw, m->fv + m->fh);
        vd[count++] = ChunkMesh::VertexData(rX, y + 1.f, rZ + 1.f, .8f, .8f, .8f, m->fu, m->fv + m->fh);
    }

    // Back
    if (m_engine->GetBlock_s(chunkX, chunkY, x, y, z - 1) == BTYPE_AIR)
    {
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y, rZ, .9f, .9f, .9f, m->du, m->dv);
        vd[count++] = ChunkMesh::VertexData(rX, y, rZ, .9f, .9f, .9f, m->du + m->dw, m->dv);
        vd[count++] = ChunkMesh::VertexData(rX, y + 1.f, rZ, .9f, .9f, .9f, m->du + m->dw, m->dv + m->dh);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y + 1.f, rZ, .9f, .9f, .9f, m->du, m->dv + m->dh);
    }

    // Right
    if (m_engine->GetBlock_s(chunkX, chunkY, x + 1, y, z) == BTYPE_AIR)
    {
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y, rZ + 1.f, .9f, .9f, .9f, m->ru, m->rv);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y, rZ, .9f, .9f, .9f, m->ru + m->rw, m->rv);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y + 1.f, rZ, .9f, .9f, .9f, m->ru + m->rw, m->rv + m->rh);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y + 1.f, rZ + 1.f, .9f, .9f, .9f, m->ru, m->rv + m->rh);
    }

    // Left
    if (m_engine->GetBlock_s(chunkX, chunkY, x - 1, y, z) == BTYPE_AIR)
    {
        vd[count++] = ChunkMesh::VertexData(rX, y, rZ, 1.f, 1.f, 1.f, m->lu, m->lv);
        vd[count++] = ChunkMesh::VertexData(rX, y, rZ + 1.f, 1.f, 1.f, 1.f, m->lu + m->lw, m->lv);
        vd[count++] = ChunkMesh::VertexData(rX, y + 1.f, rZ + 1.f, 1.f, 1.f, 1.f, m->lu + m->lw, m->lv + m->lh);
        vd[count++] = ChunkMesh::VertexData(rX, y + 1.f, rZ, 1.f, 1.f, 1.f, m->lu, m->lv + m->lh);
    }

    // Top
    if (m_engine->GetBlock_s(chunkX, chunkY, x, y + 1, z) == BTYPE_AIR)
    {
        vd[count++] = ChunkMesh::VertexData(rX, y + 1.f, rZ + 1.f, 1.f, 1.f, 1.f, m->tu, m->tv);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y + 1.f, rZ + 1.f, 1.f, 1.f, 1.f, m->tu + m->tw, m->tv);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y + 1.f, rZ, 1.f, 1.f, 1.f, m->tu + m->tw, m->tv + m->th);
        vd[count++] = ChunkMesh::VertexData(rX, y + 1.f, rZ, 1.f, 1.f, 1.f, m->tu, m->tv + m->th);
    }

    // Bottom
    if (m_engine->GetBlock_s(chunkX, chunkY, x, y - 1, z) == BTYPE_AIR)
    {
        vd[count++] = ChunkMesh::VertexData(rX, y, rZ, 1.f, 1.f, 1.f, m->bu, m->bv);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y, rZ, 1.f, 1.f, 1.f, m->bu + m->bw, m->bv);
        vd[count++] = ChunkMesh::VertexData(rX + 1.f, y, rZ + 1.f, 1.f, 1.f, 1.f, m->bu + m->bw, m->bv + m->bh);
        vd[count++] = ChunkMesh::VertexData(rX, y, rZ + 1.f, 1.f, 1.f, 1.f, m->bu, m->bv + m->bh);
    }
}

void Chunk::Reset(BlockType type)
{
    m_isDirty = true;
    m_blocks.Reset(type);
}

void Chunk::Update(float absX, float absY, int chunkX, int chunkY)
{
    // Update mesh
    if (m_isDirty)
    {
        int maxVertexCount = CHUNK_SIZE * (6 * 4);
        ChunkMesh::VertexData* vd = new ChunkMesh::VertexData[maxVertexCount];
        int count = 0;
        for (int x = 0; x < CHUNK_SIZE_WIDTH; ++x)
        {
            for (int z = 0; z < CHUNK_SIZE_DEPTH; ++z)
            {
                for (int y = 0; y < CHUNK_SIZE_HEIGHT; ++y)
                {
                    assert(count <= USHRT_MAX);

                    BlockType bt = Get(x, y, z);
                    BlockInfo* bi = m_engine->GetBlockDefinitions()->Get(bt);

                    if (bt != BTYPE_AIR)
                        AddBlockToMesh(vd, count, bi, absX, absY, chunkX, chunkY, x, y, z);
                }
            }
        }

        if (count > USHRT_MAX)
        {
            count = USHRT_MAX;
            std::cout << "[Chunk::Update] Chunk data truncated; too many vertices to have a 16-bit index" << std::endl;
        }

        m_chunkMesh.SetMeshData(vd, count);
        delete[] vd;
    }
    m_isDirty = false;
}

#undef KRYO_CHUNK_SIZE
#undef KRYO_COORDINATES_IDX

KRYO_END_NAMESPACE
