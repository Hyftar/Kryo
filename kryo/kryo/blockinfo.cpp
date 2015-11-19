#include "blockinfo.h"

KRYO_BEGIN_NAMESPACE

BlockInfo::BlockInfo() { }

BlockInfo::BlockInfo(BlockType type, const std::string& name, BlockUV uvmap)
    : m_type(type), m_name(name), m_uvmap(uvmap) { }

BlockInfo::~BlockInfo() { }

BlockType BlockInfo::GetType() const
{
    return m_type;
}

int BlockInfo::GetDurability() const
{
    return m_durability;
}

void BlockInfo::SetDurability(int durability)
{
    m_durability = durability;
}

void BlockInfo::Afficher() const
{
    std::cout << "{ Type: \"" << m_type << "\", Name: \"" << m_name << "\", Durability: \"" << m_durability << "\" }" << std::endl;
}

const BlockInfo::BlockUV* BlockInfo::GetUVMap() const
{
    return &m_uvmap;
}

KRYO_END_NAMESPACE
