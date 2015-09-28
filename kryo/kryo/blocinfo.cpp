#include "blocinfo.h"

KRYO_BEGIN_NAMESPACE

BlocInfo::BlocInfo(BlocType type, const std::string& name)
    : m_type(type), m_name(name) { }

BlocInfo::~BlocInfo() { }

BlocType BlocInfo::GetType() const
{
    return m_type;
}

int BlocInfo::GetDurabilite() const
{
    return m_durability;
}

void BlocInfo::SetDurabilite(int durability)
{
    m_durability = durability;
}

void BlocInfo::Afficher() const
{
    std::cout << "{ Type: \"" << m_type << "\", Name: \"" << m_name << "\", Durability: \"" << m_durability << "\" }" << std::endl;
}

KRYO_END_NAMESPACE
