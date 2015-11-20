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
    void ApplyRotation() const;
    void ApplyTranslation();
    Vector3f SimulateMove(bool front, bool back, bool left, bool right, bool up, bool down, float elapsedTime);
    Vector3f GetPosition() const;
    void GetChunkPosition(int& x, int& y) const;
    void SetPosition(Vector3f pos);
    void SetGravity(float v);
    void SetSpeed(Vector3f v);
    Vector3f GetSpeed() const;
    void SetSpeedX(float v);
    void SetSpeedY(float v);
    void SetSpeedZ(float v);
    float GetSpeedX();
    float GetSpeedY();
    float GetSpeedZ();
    void SetFreecam(bool v);
    bool IsFreecam() const;

private:
    bool m_freecam;
    Vector3f m_speed;
    Vector3f m_position;
    Vector3f m_chunkPosition;
    float m_rotX, m_rotY;
    float m_gravity;
};

KRYO_END_NAMESPACE

#endif
