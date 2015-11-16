#include "define.h"
#include "player.h"

KRYO_BEGIN_NAMESPACE

Player::Player(float posX, float posY, float posZ, float rotX, float rotY, Vector3f speed)
    : m_position(posX, posY, posZ), m_rotX(rotX), m_rotY(rotY), m_speed(speed), collisionX(false), collisionY(false), collisionZ(false), isGrounded(false), m_gravity(true) { }

Player::Player(Vector3f position, float rotX, float rotY, Vector3f speed)
    : m_position(position), m_rotX(rotX), m_rotY(rotY), m_speed(speed), collisionX(false), collisionY(false), collisionZ(false), isGrounded(false), m_gravity(true) { }

Player::~Player() { }

// TODO: Ajouter une variable sensibilité plutôt que l'approche ci-dessous.
void Player::TurnLeftRight(float value)
{
    m_rotY += value * CAMERA_SENSITIVITY;
    if (m_rotY > 360)
        m_rotY -= 360;
    else if (m_rotY < -360)
        m_rotY += 360;
}

// TODO: Ajouter une variable sensibilité plutôt que l'approche ci-dessous.
void Player::TurnTopBottom(float value)
{
    auto rotVal = value * CAMERA_SENSITIVITY;
    m_rotX = ((m_rotX + rotVal) <= -85) ? -85 : ((m_rotX + rotVal) >= 85 ? 85 : (m_rotX + rotVal));
}

Vector3f Player::SimulateMove(bool front, bool back, bool left, bool right, float elapsedTime)
{
    #define BLAH_SPEED 2
    float yrotrad;
    Vector3f dPosition;
    if (front ^ back)
    {
        yrotrad = (m_rotY / 180 * M_PI);
        if (front)
        {
            dPosition.x += float(sin(yrotrad)) * BLAH_SPEED /*Speed*/ * elapsedTime;
            dPosition.z -= float(cos(yrotrad)) * BLAH_SPEED  * elapsedTime;
        }
        else
        {
            dPosition.x -= float(sin(yrotrad)) * BLAH_SPEED * elapsedTime;
            dPosition.z += float(cos(yrotrad)) * BLAH_SPEED * elapsedTime;
        }
    }

    if (right ^ left)
    {
        yrotrad = (m_rotY / 180 * M_PI);
        if (right)
        {
            dPosition.x += float(cos(yrotrad)) * BLAH_SPEED * elapsedTime;
            dPosition.z += float(sin(yrotrad)) * BLAH_SPEED * elapsedTime;
        }
        else
        {
            dPosition.x -= float(cos(yrotrad)) * BLAH_SPEED * elapsedTime;
            dPosition.z -= float(sin(yrotrad)) * BLAH_SPEED * elapsedTime;
        }
    }

    // Calcul de la vitesse de chute et position en y
    /*if (collisionY)
        m_speed.y = 0;
    m_speed.y = isGrounded ? 0 : m_speed.y + GRAVACC * elapsedTime;
    dPosition.y += m_speed.y * elapsedTime;*/

    return dPosition;
}

Vector3f Player::GetPosition() const
{
    return m_position;
}

void Player::SetPosition(Vector3f pos)
{
    m_position = pos;
}

void Player::MoveFreecam(bool up, bool down, float elapsedTime)
{
    if (up ^ down)
    {
        m_position.y += 2 * elapsedTime * (up ? 1 : -1);
    }
}

void Player::ApplyRotation() const
{
    glRotatef(m_rotX, 1.f, 0, 0);
    glRotatef(m_rotY, 0, 1.f, 0);
}

void Player::ApplyTranslation()
{
    glTranslatef(-m_position.x, -m_position.y, -m_position.z);
}

KRYO_END_NAMESPACE
