// TODO: renommer 'blocinfo' en 'blockinfo'
// TODO: renommer 'SetDurabilite' et 'GetDurabilite' en 'SetDurability' et 'GetDurability, respectivement
#ifndef KRYO_BLOCKINFO
#define KRYO_BLOCKINFO

#include "global.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

class BlocInfo
{
public:
    BlocInfo(BlocType type, const std::string& name);
    ~BlocInfo();
    BlocType GetType() const;
    int GetDurabilite() const;
    void SetDurabilite(int durability);
    void Afficher() const;
private:
    BlocType m_type;
    std::string m_name;
    int m_durability = 0;
};

KRYO_END_NAMESPACE

#endif
