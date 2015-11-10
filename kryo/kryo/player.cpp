#include "define.h"
#include "player.h"

KRYO_BEGIN_NAMESPACE

Player::Player(float posX, float posY, float posZ, float rotX, float rotY, float speed)
    : m_position(Vector3f(posX, posY, posZ)), m_rotX(rotX), m_rotY(rotY), m_speed(speed) { }

Player::Player(Vector3f position, float rotX, float rotY, float speed)
    : m_position(position), m_rotX(rotX), m_rotY(rotY), m_speed(speed) { }

Player::~Player() { }

// TODO: Ajouter une variable sensibilité plutôt que l'approche ci-dessous.
void Player::TurnLeftRight(float value)
{
    m_rotY += value * CAMERA_SENSITIVITY;
    if (m_rotY > 360)
        m_rotY -= 360;
}

// TODO: Ajouter une variable sensibilité plutôt que l'approche ci-dessous.
void Player::TurnTopBottom(float value)
{
    auto rotVal = value * CAMERA_SENSITIVITY;
    m_rotX = ((m_rotX + rotVal) <= -85) ? -85 : ((m_rotX + rotVal) >= 85 ? 85 : (m_rotX + rotVal));
    if (m_rotX > 360)
        m_rotX -= 360;
}

void Player::Move(bool front, bool back, bool left, bool right, float elapsedTime)
{
    float yrotrad;
    if (front ^ back)
    {
        yrotrad = (m_rotY / 180 * M_PI);
        if (front)
        {
            m_position.x += float(sin(yrotrad)) * m_speed * elapsedTime;
            m_position.z -= float(cos(yrotrad)) * m_speed * elapsedTime;
        }
        else
        {
            m_position.x -= float(sin(yrotrad)) * m_speed * elapsedTime;
            m_position.z += float(cos(yrotrad)) * m_speed * elapsedTime;
        }
    }

    if (right ^ left)
    {
        yrotrad = (m_rotY / 180 * M_PI);
        if (right)
        {
            m_position.x += float(cos(yrotrad)) * m_speed * elapsedTime;
            m_position.z += float(sin(yrotrad)) * m_speed * elapsedTime;
        }
        else
        {
            m_position.x -= float(cos(yrotrad)) * m_speed * elapsedTime;
            m_position.z -= float(sin(yrotrad)) * m_speed * elapsedTime;
        }
    }
}

void Player::MoveFreecam(bool up, bool down, float elapsedTime)
{
    if (up ^ down)
    {
        m_position.y += m_speed * elapsedTime * (up ? 1 : -1);
    }
}

void Player::ApplyRotation() const
{
    glRotatef(m_rotX, 1.f, 0, 0);
    glRotatef(m_rotY, 0, 1.f, 0);
}

void Player::ApplyTranslation() const
{
    glTranslatef(-m_position.x, -m_position.y, -m_position.z);
}

Vector3f Player::Position() const
{
    return m_position;
}

KRYO_END_NAMESPACE
