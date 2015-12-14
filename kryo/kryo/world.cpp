#include <iomanip>
#include "global.h"
#include "player.h"
#include "vector2.h"
#include "world.h"

#define WORLD_FOLDER "../kryo/worlds/test/"

KRYO_BEGIN_NAMESPACE

World::World(Engine* e, Perlin& perlin, Player& player)
    : m_engine(e), m_loadedChunks(CHUNK_VIEW_DISTANCE * 2, CHUNK_VIEW_DISTANCE * 2), m_centerPosition(player.GetChunkPosition()), m_perlin(perlin)
{
    for (int x = 0; x < CHUNK_VIEW_DISTANCE * 2; ++x)
    {
        for (int y = 0; y < CHUNK_VIEW_DISTANCE * 2; ++y)
        {
            m_loadedChunks.Set(x, y, new Chunk(e, m_perlin, x, y));
        }
    }
}

Array2d<Chunk*>& World::GetChunks()
{
    return m_loadedChunks;
}

void World::MoveLoadedChunks(Vector2i pos)
{
    m_centerPosition = pos;
    m_minPosition = m_centerPosition - CHUNK_VIEW_DISTANCE / 4;
    m_maxPosition = m_centerPosition + CHUNK_VIEW_DISTANCE / 4;
    for (size_t i = 0; i < m_loadedChunks.GetWidth(); ++i)
        for (size_t j = 0; j < m_loadedChunks.GetHeight(); ++j)
        {
            SaveChunk(i, j);
            LoadChunk(i, j);
        }
}

void World::CheckPlayerPosition(const Player& p)
{
    auto pos = p.GetChunkPosition();
    if (pos.x < m_minPosition.x || pos.y < m_minPosition.y || pos.x > m_maxPosition.x || pos.y > m_maxPosition.y)
        MoveLoadedChunks(pos);
}

void World::SaveChunk(int x, int y)
{
    Chunk* chunk = m_loadedChunks.Get(x, y);
    if (chunk->IsModified())
    {
        std::string chunkName = WORLD_FOLDER + x + '_' + y;
        std::ofstream chunkFile(chunkName);
        chunkFile.open(chunkName, std::ios::out);
        for (size_t i = 0; i < CHUNK_SIZE_WIDTH * CHUNK_SIZE_DEPTH * CHUNK_SIZE_HEIGHT; ++i)
            chunkFile << chunk->Serialize(i) << std::endl;
    }
}

void World::LoadChunk(int x, int y)
{
    std::cout << "[" << std::setfill('0') << std::setw(2) << m_centerPosition.x + x << ", " << std::setfill('0') << std::setw(2) << m_centerPosition.y + y << "], ";
    Chunk* c = new Chunk(m_engine, m_perlin, m_centerPosition.x + x, m_centerPosition.y + y);
    std::string chunkName("" + x + '_' + y);
    if (IsWorldSaved(chunkName))
    {
        std::ifstream chunk(chunkName);
        BlockType b;
        int idx;
        while (chunk >> b)
        {
            c->Set(idx++, b);
        }
    }
    else
    {
        c->PopulateChunk(m_perlin);
    }
    m_loadedChunks.Set(x, y, c);
}

Chunk* World::Get(int x, int y) const
{
    return m_loadedChunks.Get(x, y);
}

void World::Set(int x, int y, Chunk* c)
{
    m_loadedChunks.Set(x, y, c);
}

bool World::IsWorldSaved(const std::string& name)
{
    if (FILE *file = fopen(name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}

KRYO_END_NAMESPACE
