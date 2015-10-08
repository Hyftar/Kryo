#include "player.h"

KRYO_BEGIN_NAMESPACE

Player::Player(float posX, float posY, float posZ, float rotX, float rotY, float speed)
    : m_posX(posX), m_posY(posY), m_posZ(posZ), m_rotX(rotX), m_rotY(rotY), m_speed(speed) { }

Player::~Player() { }

void Player::TurnLeftRight(float value)
{
    m_rotY += value * 0.3;
    if (m_rotY > 360)
        m_rotY -= 360;
}

void Player::TurnTopBottom(float value)
{
    m_rotX += value * 0.3;
    if (m_rotX > 360)
        m_rotX -= 360;
}

void Player::Move(bool front, bool back, bool left, bool right, float elapsedTime)
{
    float xrotrad, yrotrad;
    if (front ^ back)
    {
        yrotrad = (m_rotY / 180 * M_PI);
        if (front)
        {
            m_posX -= float(sin(yrotrad)) * m_speed * elapsedTime;
            m_posZ += float(cos(yrotrad)) * m_speed * elapsedTime;
        }
        else
        {
            m_posX += float(sin(yrotrad)) * m_speed * elapsedTime;
            m_posZ -= float(cos(yrotrad)) * m_speed * elapsedTime;
        }
    }

    if (right ^ left)
    {
        yrotrad = (m_rotY / 180 * M_PI);
        if (right)
        {
            m_posX -= float(cos(yrotrad)) * m_speed * elapsedTime;
            m_posZ -= float(sin(yrotrad)) * m_speed * elapsedTime;
        }
        else
        {
            m_posX += float(cos(yrotrad)) * m_speed * elapsedTime;
            m_posZ += float(sin(yrotrad)) * m_speed * elapsedTime;
        }
    }
}

void Player::ApplyRotation() const
{
    glRotatef(m_rotX, 1.f, 0, 0);
    glRotatef(m_rotY, 0, 1.f, 0);
}

void Player::ApplyTranslation() const
{
    glTranslatef(m_posX, m_posY, m_posZ);
}

KRYO_END_NAMESPACE
