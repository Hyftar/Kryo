#ifndef KRYO_PLAYER_H
#define KRYO_PLAYER_H

#include "global.h"

KRYO_BEGIN_NAMESPACE

class Player
{
public:
    Player(float posX = 0, float posY = 0, float posZ = 0, float rotX = 0, float rotY = 0, float speed = 2);
    ~Player();
    void TurnLeftRight(float value);
    void TurnTopBottom(float value);
    void Move(bool front, bool back, bool left, bool right, float elapsedTime);
    void ApplyRotation() const;
    void ApplyTranslation() const;

private:
    float m_posX, m_posY, m_posZ;
    float m_rotX, m_rotY;
    float m_speed;
};

KRYO_END_NAMESPACE

#endif
