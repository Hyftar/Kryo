#ifndef KRYO_CHUNK_H
#define KRYO_CHUNK_H

#include "global.h"
#include "blocarray3d.h"

KRYO_BEGIN_NAMESPACE

class Chunk : public BlocArray3d
{
public:
    Chunk();
    ~Chunk();
    void RemoveBloc(int x, int y, int z);
    void SetBloc(int x, int y, int z, BlocType type);
    BlocType GetBloc(int x, int y, int z) const;
};

KRYO_END_NAMESPACE

#endif
