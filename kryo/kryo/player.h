#ifndef KRYO_PLAYER_H
#define KRYO_PLAYER_H

#include "global.h"
#include "vector3.h"

KRYO_BEGIN_NAMESPACE

class Player
{
public:
    Player(float posX = 0, float posY = 0, float posZ = 0, float rotX = 0, float rotY = 0, float speed = 2);
    Player(Vector3f pos, float rotX = 0, float rotY = 0, float speed = 2);
    ~Player();
    void TurnLeftRight(float value);
    void TurnTopBottom(float value);
    void Move(bool front, bool back, bool left, bool right, float elapsedTime);
    void ApplyRotation() const;
    void ApplyTranslation() const;
    // TODO: renommer par GetPosition()
    Vector3f Position() const;

private:
    Vector3f m_position;
    float m_rotX, m_rotY;
    float m_speed;
};

KRYO_END_NAMESPACE

#endif
