#ifndef KRYO_BLOCKINFO_H
#define KRYO_BLOCKINFO_H

#include "global.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

class BlockInfo
{
public:
    struct BlockUV
    {
        float lu, lv, lw, lh;
        float ru, rv, rw, rh;
        float tu, tv, tw, th;
        float bu, bv, bw, bh;
        float fu, fv, fw, fh;
        float du, dv, dw, dh;
    };

    BlockInfo();
    BlockInfo(BlockType type, const std::string& name, BlockUV uvmap, const bool solid);
    ~BlockInfo();
    BlockType GetType() const;
    int GetDurability() const;
    void SetDurability(int durability);
    void Afficher() const;
    BlockUV GetUVMap() const;
    bool IsSolid() const;

private:
    BlockUV m_uvmap;
    BlockType m_type;
    std::string m_name;
    float WalkingSpeed;
    int m_durability;
    bool m_solid;
};

KRYO_END_NAMESPACE

#endif

