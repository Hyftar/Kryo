// TODO: renommer 'blocarray3d' et 'BlocArray3d' en 'blockarray3d' et 'BlockArray3D', respectivement
#ifndef KRYO_BLOCKARRAY3D
#define KRYO_BLOCKARRAY3D

#include "global.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

class BlocArray3d
{
public:
    BlocArray3d(int width, int height, int depth);
    BlocArray3d(const BlocArray3d &source);
    virtual ~BlocArray3d();
    void Set(int x, int y, int z, BlocType type);
    void Set(int idx, BlocType type);
    BlocType Get(int x, int y, int z) const;
    BlocType Get(int idx) const;
    int GetBlockCount() const;
    void Reset(BlocType type);
private:
    int m_width;
    int m_height;
    int m_depth;
    BlocType* m_blocks;
};

KRYO_END_NAMESPACE

#endif
