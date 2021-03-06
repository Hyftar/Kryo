#ifndef KRYO_ENGINE_H
#define KRYO_ENGINE_H

#include "global.h"
#include "define.h"
#include "vector3.h"
#include "array2d.h"
#include "texture.h"
#include "shader.h"
#include "textureatlas.h"
#include "blockinfo.h"
#include "openglcontext.h"
#include "world.h"
#include "player.h"
#include "tool.h"

KRYO_BEGIN_NAMESPACE

class World;
class Player;

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

    Chunk* GetChunk_s(int chunkX, int chunkY, int nx, int ny, int nz) const;
    void SetBlock_s(int chunkX, int chunkY, int nx, int ny, int nz, BlockType bt) const;
    void SetBlock_s(int nx, int ny, int nz, BlockType type) const;
    void SetBlock_s(int chunkX, int chunkY, Vector3i v, BlockType type) const;
    void SetBlock_s(Vector3i v, BlockType type) const;
    BlockType GetBlock_s(int chunkX, int chunkY, int nx, int ny, int nz) const;
    BlockType GetBlock_s(int xn, int ny, int nz) const;
    BlockType GetBlock_s(int chunkX, int chunkY, Vector3i v) const;
    BlockType GetBlock_s(Vector3i v) const;

    Array2d<BlockInfo*>* GetBlockDefinitions() { return &m_blockDefinitions; }
    const Array2d<BlockInfo*>* GetBlockDefinitions() const { return &m_blockDefinitions; }

    void LoadChunks();
private:
    int GetFps() const;
    void PrintText(unsigned int x, unsigned int y, const std::string & t) const;
    void DrawHud() const;
    void GetBlockAtCursor();
    void CheckCollisions(Player& player, Vector3f movement);
    void DrawCube(int x, int y, int z, float rotX = 0, float rotY = 0, float rotZ = 0);
    void DrawHexagon(int x, int y, int z, float rotX = 0, float rotY = 0, float rotZ = 0);
    BlockInfo::BlockUV* CreateBlockUV(
        TextureAtlas::TextureIndex frontIndex, TextureAtlas::TextureIndex backIndex,
        TextureAtlas::TextureIndex rightIndex, TextureAtlas::TextureIndex leftIndex,
        TextureAtlas::TextureIndex topIndex, TextureAtlas::TextureIndex bottomIndex) const;
    void AddBlockDefinition(const BlockType bt, const std::string& name,
        const std::string& frontPath, const std::string& backPath,
        const std::string& rightPath, const std::string& leftPath,
        const std::string& topPath,   const std::string& bottomPath, const bool solid = true);
    void AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& path, const bool solid = true);
    void AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& topPath, const std::string& sidePath, const bool solid = true);
    void AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& topPath, const std::string& frontPath, const std::string& sidePath, const bool solid = true);
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

private:
    bool m_wireframe;
    bool m_moveForward, m_moveBackward, m_moveLeft, m_moveRight, m_moveUp, m_moveDown;

    Vector3i m_currentBlock;
    Vector3f m_currentFaceNormal;

    Perlin m_perlin;

    Player m_player;
    World m_chunks;
    Array2d<BlockInfo*> m_blockDefinitions;

    Texture m_textureFont, m_textureCrosshair;
    TextureAtlas m_textureAtlas;
    Shader m_shader01;
    int m_fps;
};

KRYO_END_NAMESPACE

#endif
