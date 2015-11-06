#ifndef KRYO_BLOCKINFO_H
#define KRYO_BLOCKINFO_H

#include "global.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

class BlockInfo
{
public:
    BlockInfo(BlockType type, const std::string& name);
    ~BlockInfo();
    BlockType GetType() const;
    int GetDurability() const;
    void SetDurability(int durability);
    void Afficher() const;

private:
    BlockType m_type;
    std::string m_name;
    int m_durability;
};

KRYO_END_NAMESPACE

#endif
