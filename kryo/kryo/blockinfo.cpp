#include "blockinfo.h"

KRYO_BEGIN_NAMESPACE

BlockInfo::BlockInfo(BlockType type, const std::string& name)
    : m_type(type), m_name(name) { }

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

KRYO_END_NAMESPACE
