#ifndef KRYO_ENGINE_H
#define KRYO_ENGINE_H

#include "global.h"
#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "player.h"

KRYO_BEGIN_NAMESPACE

class Engine : public OpenglContext
{
public:
    Engine();
    virtual ~Engine();
    virtual void Init();
    virtual void DeInit();
    virtual void LoadResource();
    virtual void UnloadResource();
    virtual void Render(float elapsedTime);
    virtual void KeyPressEvent(unsigned char key);
    virtual void KeyReleaseEvent(unsigned char key);
    virtual void MouseMoveEvent(int x, int y);
    virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
    virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

private:
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);
    void DrawCube(int x, int y, int z, float rotX = 0, float rotY = 0, float rotZ = 0);
    void DrawHexagon(int x, int y, int z, float rotX = 0, float rotY = 0, float rotZ = 0);

private:
    bool m_wireframe;
    bool m_moveUp, m_moveDown, m_moveLeft, m_moveRight;

    Texture m_textureFloor;
    Texture m_textureCube;

    Player m_player;
};

KRYO_END_NAMESPACE

#endif
