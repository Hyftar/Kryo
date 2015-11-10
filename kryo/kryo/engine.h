#ifndef KRYO_ENGINE_H
#define KRYO_ENGINE_H

#include "global.h"
#include "define.h"
#include "player.h"
#include "chunk.h"
#include "texture.h"
#include "shader.h"
#include "openglcontext.h"
#include "blockinfo.h"
#include "textureatlas.h"
#include "array2d.h"

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

    void SetFreecam(bool freecam);
    bool IsFreecam() const;
private:
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);
    void DrawCube(int x, int y, int z, float rotX = 0, float rotY = 0, float rotZ = 0);
    void DrawHexagon(int x, int y, int z, float rotX = 0, float rotY = 0, float rotZ = 0);
    void AddBlockDefinition(const BlockType bt, const std::string& name,
        const std::string& frontPath, const std::string& backPath,
        const std::string& rightPath, const std::string& leftPath,
        const std::string& topPath,   const std::string& bottomPath);

    void AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& path);
    void AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& topPath, const std::string& sidePath);
    void AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& topPath, const std::string& frontPath, const std::string& sidePath);
private:
    bool m_wireframe;
    bool m_moveForward, m_moveBackward, m_moveLeft, m_moveRight, m_moveUp, m_moveDown;
    bool m_freeCam;

    Player m_player;
    Chunk m_testChunk;
    Array2d<BlockInfo> m_blockDefinitions;

    TextureAtlas m_textureAtlas;
    Shader m_shader01;
};

KRYO_END_NAMESPACE

#endif
