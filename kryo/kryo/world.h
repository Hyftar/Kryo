#ifndef KRYO_WORLD_H
#define KRYO_WORLD_H

#include "global.h"
#include "chunk.h"
#include "define.h"
#include "player.h"
#include <fstream>

KRYO_BEGIN_NAMESPACE

class World
{
public:
    World(Engine* e, Perlin& perlin ,Player& player);
    Array2d<Chunk*>& GetChunks();
    void MoveLoadedChunks(Vector2i pos);
    void CheckPlayerPosition(const Player& p);
    void SaveChunk(int x, int y);
    void LoadChunk(int x, int y);
    Chunk* Get(int x, int y) const;
    void Set(int x, int y, Chunk* c);

    bool IsWorldSaved(const std::string & name);

private:
    Engine* m_engine;

    Vector2i m_maxPosition;
    Vector2i m_minPosition;
    Vector2i m_centerPosition;

    Vector2i m_loadedMaxPosition;
    Vector2i m_loadedMinPosition;

    Array2d<Chunk*> m_loadedChunks;

    Perlin& m_perlin;
};

KRYO_END_NAMESPACE

#endif
