#include "blockinfo.h"
#include "chunk.h"
#include "engine.h"

KRYO_BEGIN_NAMESPACE

#define KRYO_CHUNK_SIZE (m_width * m_height * m_depth)
#define KRYO_COORDINATES_IDX(x, y, z) (x + (z * m_width) + (y * m_depth * m_width))

Chunk::Chunk(int x, int y, int z)
    : m_isDirty(false), m_width(x), m_height(y), m_depth(z), m_blocks(x, y, z)
{
    PopulateArrayTest();
}

Chunk::Chunk(Chunk &source)
    : m_isDirty(false), m_width(source.m_width), m_height(source.m_height),
    m_depth(source.m_depth), m_blocks(Array3d<BlockType>(source.m_blocks)) { }

Chunk::~Chunk() { }

void Chunk::PopulateArrayTest()
{
    m_blocks.Reset(BTYPE_AIR);
    for (int x = 0; x < CHUNK_SIZE_WIDTH; ++x)
    {
        for (int z = 0; z < CHUNK_SIZE_DEPTH; ++z)
        {
            for (int y = 0; y < 1; ++y)
            {
                m_blocks.Set(x, y, z, BTYPE_GRASS);
            }
        }
    }

    for (size_t i = 0; i < 5; ++i)
    {
        m_blocks.Set(8, 1 + rand() % 4, i + 1, BTYPE_GRASS);
    }

    m_isDirty = true;
}

void Chunk::Remove(int idx)
{
    m_blocks.Set(idx, BTYPE_AIR);
}

void Chunk::Remove(int x, int y, int z)
{
    Set(KRYO_COORDINATES_IDX(x, y, z), BTYPE_AIR);
}

void Chunk::Set(int idx, BlockType type)
{
    m_isDirty = true;
    m_blocks.Set(idx, type);
}

void Chunk::Set(int x, int y, int z, BlockType type)
{
    Set(KRYO_COORDINATES_IDX(x, y, z), type);
}

void Chunk::Render() const
{
    m_chunkMesh.Render();
}

bool Chunk::IsDirty() const
{
    return m_isDirty;
}

BlockType Chunk::Get(int idx) const
{
    return m_blocks.Get(idx);
}

BlockType Chunk::Get(int x, int y, int z) const
{
    return Get(KRYO_COORDINATES_IDX(x, y, z));
}

void Chunk::AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, BlockInfo bi, int x, int y, int z)
{
    BlockInfo::BlockUV m = bi.GetUVMap();

    // Front
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z + .5f, .8f, .8f, .8f, m.fu, m.fv);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z + .5f, .8f, .8f, .8f, m.fu + m.fw, m.fv);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z + .5f, .8f, .8f, .8f, m.fu + m.fw, m.fv + m.fh);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z + .5f, .8f, .8f, .8f, m.fu, m.fv + m.fh);

    // Back
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z - .5f, .9f, .9f, .9f, m.du, m.dv);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z - .5f, .9f, .9f, .9f, m.du + m.dw, m.dv);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z - .5f, .9f, .9f, .9f, m.du + m.dw, m.dv + m.dh);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z - .5f, .9f, .9f, .9f, m.du, m.dv + m.dh);

    // Right
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z + .5f, .9f, .9f, .9f, m.ru, m.rv);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z - .5f, .9f, .9f, .9f, m.ru + m.rw, m.rv);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z - .5f, .9f, .9f, .9f, m.ru + m.rw, m.rv + m.rh);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z + .5f, .9f, .9f, .9f, m.ru, m.rv + m.rh);

    // Left
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, m.lu, m.lv);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, m.lu + m.lw, m.lv);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, m.lu + m.lw, m.lv + m.lh);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, m.lu, m.lv + m.lh);

    // Top
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, m.tu, m.tv);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, m.tu + m.tw, m.tv);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, m.tu + m.tw, m.tv + m.th);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, m.tu, m.tv + m.th);

    // Bottom
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, m.bu, m.bv);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, m.bu + m.bw, m.bv);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, m.bu + m.bw, m.bv + m.bh);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, m.bu, m.bv + m.bh);
}

void Chunk::Reset(BlockType type)
{
    m_isDirty = true;
    m_blocks.Reset(type);
}

void Chunk::Update(Engine* engine)
{
    // Update mesh
    if (m_isDirty)
    {
        int maxVertexCount = KRYO_CHUNK_SIZE * (6 * 4);
        ChunkMesh::VertexData* vd = new ChunkMesh::VertexData[maxVertexCount];
        int count = 0;
        for (int x = 0; x < CHUNK_SIZE_WIDTH; ++x)
        {
            for (int z = 0; z < CHUNK_SIZE_DEPTH; ++z)
            {
                for (int y = 0; y < CHUNK_SIZE_HEIGHT; ++y)
                {
                    if (count > USHRT_MAX)
                        break;
                    BlockType bt = Get(x, y, z);
                    BlockInfo bi = engine->GetBlockDefinitions().Get(bt);
                    if (bt != BTYPE_AIR)
                        AddBlockToMesh(vd, count, bi, x, y, z);
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

int Chunk::GetBlockCount() const
{
    return KRYO_CHUNK_SIZE;
}

#undef KRYO_CHUNK_SIZE
#undef KRYO_COORDINATES_IDX

KRYO_END_NAMESPACE
