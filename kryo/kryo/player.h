#ifndef KRYO_PLAYER_H
#define KRYO_PLAYER_H

#include "global.h"
#include "vector3.h"
#include "chunk.h"

KRYO_BEGIN_NAMESPACE

class Player
{
public:
    Player(float posX = 0, float posY = 0, float posZ = 0, float rotX = 0, float rotY = 0, Vector3f speed = Vector3f());
    Player(Vector3f pos, float rotX = 0, float rotY = 0, Vector3f speed = Vector3f());
    ~Player();
    void TurnLeftRight(float value);
    void TurnTopBottom(float value);
    void Move(bool front, bool back, bool left, bool right, float elapsedTime);
    void MoveFreecam(bool up, bool down, float elapsedTime);
    void ApplyRotation() const;
    void ApplyTranslation();
    Vector3f SimulateMove(bool front, bool back, bool left, bool right, float elapsedTime);
    Vector3f GetPosition() const;
    void SetPosition(Vector3f pos);
    void SetGravity(bool v);
public:
    bool collisionX, collisionY, collisionZ, isGrounded;

private:
    Vector3f m_position;
    Vector3f m_speed;
    Vector3f m_chunkPosition;
    float m_rotX, m_rotY;
    bool m_gravity;
};

KRYO_END_NAMESPACE

#endif
