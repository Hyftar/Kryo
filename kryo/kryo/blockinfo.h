// TODO: renommer 'SetDurabilite' et 'GetDurabilite' en 'SetDurability' et 'GetDurability, respectivement
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
    int GetDurabilite() const;
    void SetDurabilite(int durability);
    void Afficher() const;

private:
    BlockType m_type;
    std::string m_name;
    int m_durability;
};

KRYO_END_NAMESPACE

#endif
