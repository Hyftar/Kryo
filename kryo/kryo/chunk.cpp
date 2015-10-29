#include "chunk.h"

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
            for (int y = 0; y < 32; ++y)
            {
                if (x % 2 == 0 && y % 2 == 0 && z % 2 == 0)
                    m_blocks.Set(x, y, z, BTYPE_DIRT);
            }
        }
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
    Set(KRYO_COORDINATES_IDX(x, y,z), type);
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

void Chunk::AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, BlockType bt, int x, int y, int z)
{
    // Front
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z + .5f, .8f, .8f, .8f, 0.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z + .5f, .8f, .8f, .8f, 1.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z + .5f, .8f, .8f, .8f, 1.f, 1.f);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z + .5f, .8f, .8f, .8f, 0.f, 1.f);

    // Back
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z - .5f, .9f, .9f, .9f, 0.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z - .5f, .9f, .9f, .9f, 1.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z - .5f, .9f, .9f, .9f, 1.f, 1.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z - .5f, .9f, .9f, .9f, 0.f, 1.f);

    // Right
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z + .5f, .9f, .9f, .9f, 0.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z - .5f, .9f, .9f, .9f, 1.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z - .5f, .9f, .9f, .9f, 1.f, 1.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z + .5f, .9f, .9f, .9f, 0.f, 1.f);

    // Left
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, 0.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, 1.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, 1.f, 1.f);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, 0.f, 1.f);

    // Top
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, 0.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, 1.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, 1.f, 1.f);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, 0.f, 1.f);

    // Bottom
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, 0.f, 1.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, 1.f, 1.f);
    vd[count++] = ChunkMesh::VertexData(x + .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, 1.f, 0.f);
    vd[count++] = ChunkMesh::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, 0.f, 0.f);
}

void Chunk::Reset(BlockType type)
{
    m_isDirty = true;
    m_blocks.Reset(type);
}

void Chunk::Update()
{
    // Update mesh
    if (m_isDirty)
    {
        int maxVertexCount = (CHUNK_SIZE_WIDTH * CHUNK_SIZE_HEIGHT * CHUNK_SIZE_DEPTH) * (6 * 4);
        ChunkMesh::VertexData * vd = new ChunkMesh::VertexData[maxVertexCount];
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
                    if (bt != BTYPE_AIR)
                        AddBlockToMesh(vd, count, bt, x, y, z);
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
