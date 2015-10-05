#ifndef KRYO_ENGINE_H
#define KRYO_ENGINE_H

#include "global.h"
#include "define.h"
#include "openglcontext.h"
#include "texture.h"

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

private:
    bool m_wireframe;
    Texture m_textureFloor;
};

KRYO_END_NAMESPACE

#endif
