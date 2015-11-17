#include "engine.h"

KRYO_BEGIN_NAMESPACE

Engine::Engine() : m_wireframe(false), m_player(8, 20, 3, 90),
m_moveForward(false), m_moveBackward(false), m_moveLeft(false), m_moveRight(false), m_moveUp(false), m_moveDown(false), m_blockDefinitions(Array2d<BlockInfo>(4, 4)), m_textureAtlas(4) { }

Engine::~Engine() { }

void Engine::Init()
{
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << " Error while initializing GLEW... aborting (" << glewGetErrorString(err) << ") " << std::endl;
        abort();
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)Width() / (float)Height(), 0.1f, 1000.0f);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_CULL_FACE);

    // Light
    GLfloat light0Pos[4] = { 0.0f, CHUNK_SIZE_HEIGHT, 0.0f, 1.0f };
    GLfloat light0Amb[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat light0Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light0Spec[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

    CenterMouse();
    HideCursor();
}

void Engine::DeInit()
{
}

void Engine::AddBlockDefinition(const BlockType bt, const std::string& name,
    const std::string& frontPath, const std::string& backPath,
    const std::string& rightPath, const std::string& leftPath,
    const std::string& topPath, const std::string& bottomPath)
{
    TextureAtlas::TextureIndex
        frontIndex = m_textureAtlas.AddTexture(frontPath),
        backIndex = m_textureAtlas.AddTexture(backPath),
        rightIndex = m_textureAtlas.AddTexture(rightPath),
        leftIndex = m_textureAtlas.AddTexture(leftPath),
        topIndex = m_textureAtlas.AddTexture(topPath),
        bottomIndex = m_textureAtlas.AddTexture(bottomPath);

    BlockInfo::BlockUV* m = new BlockInfo::BlockUV();
    m_textureAtlas.TextureIndexToCoord(frontIndex, m->fu, m->fv, m->fw, m->fh);
    m_textureAtlas.TextureIndexToCoord(backIndex, m->du, m->dv, m->dw, m->dh);
    m_textureAtlas.TextureIndexToCoord(rightIndex, m->ru, m->rv, m->rw, m->rh);
    m_textureAtlas.TextureIndexToCoord(leftIndex, m->lu, m->lv, m->lw, m->lh);
    m_textureAtlas.TextureIndexToCoord(topIndex, m->tu, m->tv, m->tw, m->th);
    m_textureAtlas.TextureIndexToCoord(bottomIndex, m->bu, m->bv, m->bw, m->bh);

    m_blockDefinitions.Set(bt, BlockInfo(bt, name, *m, true));
}

void Engine::AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& path)
{
    return AddBlockDefinition(bt, name, path, path, path, path, path, path);
}

void Engine::AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& topPath, const std::string& sidePath)
{
    return AddBlockDefinition(bt, name, sidePath, sidePath, sidePath, sidePath, topPath, sidePath);
}

void Engine::AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& topPath, const std::string& frontPath, const std::string& sidePath)
{
    return AddBlockDefinition(bt, name, frontPath, sidePath, sidePath, sidePath, topPath, sidePath);
}

void Engine::LoadResource()
{
    //TextureAtlas::TextureIndex texCheckerIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "checker.bmp");
    AddBlockDefinition(BTYPE_DIRT, "Dirt", TEXTURE_PATH "dirt.png");
    AddBlockDefinition(BTYPE_GRASS, "Grass",
        TEXTURE_PATH "grass_side.png",
        TEXTURE_PATH "grass_side.png",
        TEXTURE_PATH "grass_side.png",
        TEXTURE_PATH "grass_side.png",
        TEXTURE_PATH "grass.png",
        TEXTURE_PATH "dirt.png");

    if (!m_textureAtlas.Generate(128, false))
    {
        std::cout << "Unable to generate texture atlas..." << std::endl;
        abort();
    }

    std::cout << "Loading and compiling shaders..." << std::endl;
    if (!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
    {
        std::cout << "Failed to load shaders" << std::endl;
        exit(1);
    }
}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{
    static float gameTime = elapsedTime;

    gameTime += elapsedTime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Transformations initiales
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vector3f dPosition = m_player.SimulateMove(m_moveForward, m_moveBackward, m_moveLeft, m_moveRight, m_moveUp, m_moveDown, elapsedTime);
    CheckCollisions(m_player, dPosition);

    m_player.ApplyRotation();
    m_player.ApplyTranslation();

    m_textureAtlas.Bind();

    // Plancher
    // Les vertex doivent etre affiches dans le sens anti-horaire (CCW)
    float nbRep = 50.f;
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0); // Normal vector
    glTexCoord2f(0, 0);
    glVertex3f(-100.f, -2.f, 100.f);
    glTexCoord2f(nbRep, 0);
    glVertex3f(100.f, -2.f, 100.f);
    glTexCoord2f(nbRep, nbRep);
    glVertex3f(100.f, -2.f, -100.f);
    glTexCoord2f(0, nbRep);
    glVertex3f(-100.f, -2.f, -100.f);
    glEnd();

    if (m_testChunk.IsDirty())
        m_testChunk.Update(this);

    m_shader01.Use();
    m_testChunk.Render();
    Shader::Disable();

    //DrawHexagon(0, -1, -7);
}

void Engine::KeyPressEvent(unsigned char key)
{
    switch (key)
    {
    case 36: // ESC
        Stop();
        break;
    case 94: // F10
        SetFullscreen(!IsFullscreen());
        break;
    case 87: // F3
        m_player.SetFreecam(!m_player.IsFreecam());
        break;
    case 0: // A
        m_moveLeft = true;
        break;
    case 22: // W
        m_moveForward = true;
        break;
    case 18: // S
        m_moveBackward = true;
        break;
    case 3: // D
        m_moveRight = true;
        break;
    case 16: // Q
        m_moveDown = true;
        break;
    case 4: // E
        m_moveUp = true;
        break;
    case 24: // Y
        m_wireframe = !m_wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, (m_wireframe ? GL_LINE : GL_FILL));
        break;
    case 57: // Space
        if (!m_player.IsFreecam() && m_player.GetSpeedY() == 0)
            m_player.SetSpeedY(6.f);
        break;
    default:
        std::cout << "Unhandled key: " << (int)key << std::endl;
    }
}

void Engine::KeyReleaseEvent(unsigned char key)
{
    switch (key)
    {
    case 0: // A
        m_moveLeft = false;
        break;
    case 22: // W
        m_moveForward = false;
        break;
    case 18: // S
        m_moveBackward = false;
        break;
    case 3: // D
        m_moveRight = false;
        break;
    case 16: // Q
        m_moveDown = false;
        break;
    case 4: // E
        m_moveUp = false;
        break;
    }
}

void Engine::MouseMoveEvent(int x, int y)
{
    // Centrer la souris seulement si elle n'est pas déjà centrée
    // Il est nécessaire de faire la vérification pour éviter de tomber
    // dans une boucle infinie où l'appel à CenterMouse génère un
    // MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre
    // MouseMoveEvent, etc
    int posX = Width() / 2,
        posY = Height() / 2;

    if (x == posX && y == posY)
        return;

    CenterMouse();
    MakeRelativeToCenter(x, y);

    if (x != posX)
        m_player.TurnLeftRight(x);

    if (y != posY)
        m_player.TurnTopBottom(y);
}

void Engine::MousePressEvent(const MOUSE_BUTTON& button, int x, int y)
{
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y)
{
}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string & t)
{
    glLoadIdentity();
    glTranslated(x, y, 0);
    for (unsigned int i = 0; i<t.length(); ++i)
    {
        float left = (float)((t[i] - 32) % 16) / 16.0f;
        float top = (float)((t[i] - 32) / 16) / 16.0f;
        top += 0.5f;
        glBegin(GL_QUADS);
            glTexCoord2f(left, 1.0f - top - 0.0625f);
            glVertex2f(0, 0);
            glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
            glVertex2f(12, 0);
            glTexCoord2f(left + 0.0625f, 1.0f - top);
            glVertex2f(12, 12);
            glTexCoord2f(left, 1.0f - top);
            glVertex2f(0, 12);
        glEnd();
        glTranslated(8, 0, 0);
    }
}

void Engine::DrawHud() const
{
    //// Setter le blend function , tout ce qui sera noir sera transparent
    //glDisable(GL_LIGHTING);
    //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glEnable(GL_BLEND);
    //glDisable(GL_DEPTH_TEST);
    //glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    //    glLoadIdentity();
    //    glOrtho(0, Width(), 0, Height(), -1, 1);
    //    glMatrixMode(GL_MODELVIEW);
    //    glPushMatrix();
    //        // Bind de la texture pour le font
    //        m_textureFont.Bind();
    //        std::ostringstream ss;
    //        ss << "Fps : " << GetFps();
    //        PrintText(10, Height() - 25, ss.str());
    //        ss.str("");
    //        ss << "Position : " << m_player.GetPosition();
    //        PrintText(10, 10, ss.str());

    //        // Affichage du crosshair
    //        m_textureCrosshair.Bind();
    //        static const int crossSize = 32;
    //        glLoadIdentity();
    //        glTranslated(Width() / 2 - crossSize / 2, Height() / 2 - crossSize / 2, 0);
    //        glBegin(GL_QUADS);
    //            glTexCoord2f(0, 0);
    //            glVertex2i(0, 0);
    //            glTexCoord2f(1, 0);
    //            glVertex2i(crossSize, 0);
    //            glTexCoord2f(1, 1);
    //            glVertex2i(crossSize, crossSize);
    //            glTexCoord2f(0, 1);
    //            glVertex2i(0, crossSize);
    //        glEnd();
    //        glEnable(GL_LIGHTING);
    //        glDisable(GL_BLEND);
    //        glEnable(GL_DEPTH_TEST);
    //        glMatrixMode(GL_PROJECTION);
    //    glPopMatrix();
    //    glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();
}

// REMOVE THIS
BlockType Engine::Get_s(int x, int y, int z)
{
    if (x < 0 || x >= CHUNK_SIZE_WIDTH || y < 0 || y >= CHUNK_SIZE_HEIGHT || z < 0 || z >= CHUNK_SIZE_DEPTH)
        return BTYPE_AIR;

    return m_testChunk.Get(x, y, z);
}

void Engine::CheckCollisions(Player& player, Vector3f movement)
{
    /*if (movement == Vector3f())
        return;*/

    //std::cout << "falling" << std::endl;

    Vector3f playerPos = player.GetPosition();
    Vector3f expectedPos = playerPos + movement;

    #define PLAYER_HEIGHT 1.7f
    #define BLOCK_HEIGHT 1.f
    #define BLOCK_MARGIN .15f
    #define BLOCK_EPSILON .0001f

    /*auto aposY = playerPos.y;
    if (m_testChunk.Get(int(expectedPos.x - BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(expectedPos.x + BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(expectedPos.x - BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT + BLOCK_HEIGHT, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(expectedPos.x + BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT + BLOCK_HEIGHT, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(expectedPos.x + BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(expectedPos.x - BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(expectedPos.x + BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT + BLOCK_HEIGHT, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(expectedPos.x - BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT + BLOCK_HEIGHT, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR)
    {
        movement.x = 0;
    }

    if (m_testChunk.Get(int(playerPos.x - BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(playerPos.x + BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(playerPos.x - BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT + BLOCK_HEIGHT, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(playerPos.x + BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT + BLOCK_HEIGHT, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(playerPos.x + BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(playerPos.x - BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(playerPos.x + BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT + BLOCK_HEIGHT, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        m_testChunk.Get(int(playerPos.x - BLOCK_MARGIN), int(aposY - PLAYER_HEIGHT) + BLOCK_HEIGHT + BLOCK_HEIGHT, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR)
    {
        movement.z = 0;
    }*/

    if (playerPos != expectedPos)
    {
        bool positive = playerPos.y <= expectedPos.y;
        int delta = roundf(abs(playerPos.y - expectedPos.y));
        int i = delta;

        if (delta != 0)
        {
            std::cout << "";
        }
        do
        {
            float blockY = positive ? int(playerPos.y + BLOCK_HEIGHT) + i : playerPos.y - (delta - i) - PLAYER_HEIGHT;
            float posY = positive ? playerPos.y : playerPos.y;

            std::cout << blockY << " | " << posY << std::endl;

            auto btL = Get_s(int(playerPos.x + BLOCK_MARGIN), int(blockY - BLOCK_HEIGHT), int(playerPos.z));
            auto btR = Get_s(int(playerPos.x - BLOCK_MARGIN), int(blockY - BLOCK_HEIGHT), int(playerPos.z));
            auto btF = Get_s(int(playerPos.x), int(blockY- BLOCK_HEIGHT), int(playerPos.z + BLOCK_MARGIN));
            auto btB = Get_s(int(playerPos.x), int(blockY- BLOCK_HEIGHT), int(playerPos.z - BLOCK_MARGIN));

            auto btD1 = Get_s(int(playerPos.x - BLOCK_MARGIN), int(blockY), int(playerPos.z - BLOCK_MARGIN));
            auto btD2 = Get_s(int(playerPos.x - BLOCK_MARGIN), int(blockY), int(playerPos.z + BLOCK_MARGIN));
            auto btD3 = Get_s(int(playerPos.x + BLOCK_MARGIN), int(blockY), int(playerPos.z - BLOCK_MARGIN));
            auto btD4 = Get_s(int(playerPos.x + BLOCK_MARGIN), int(blockY), int(playerPos.z + BLOCK_MARGIN));

            auto diff = floorf(blockY);
            std::cout << "Diff: " << diff << std::endl;
            if ((btL != BTYPE_AIR || btR != BTYPE_AIR || btF != BTYPE_AIR || btB != BTYPE_AIR
                || btD1 != BTYPE_AIR || btD2 != BTYPE_AIR || btD3 != BTYPE_AIR || btD4 != BTYPE_AIR)
                && (positive ? ((posY - BLOCK_MARGIN) - blockY) < BLOCK_EPSILON : diff < BLOCK_EPSILON))
            {
                float newPos = positive ? posY - BLOCK_MARGIN : floorf(blockY) + BLOCK_HEIGHT + PLAYER_HEIGHT;
                //std::cout << "It's a hit? " << posY << " | " << newPos << std::endl;
                player.SetPosition(Vector3f(playerPos.x + movement.x, newPos, playerPos.z + movement.z));
                player.SetSpeedY(0);
                return;
            }
        } while (i--);
    }
    player.SetPosition(playerPos + movement);
}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
    texture.Load(filename);
    if (!texture.IsValid())
    {
        std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
        if (stopOnError)
            Stop();

        return false;
    }

    return true;
}

void Engine::DrawHexagon(int x, int y, int z, float rotX, float rotY, float rotZ)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    if (rotX != 0)
        glRotatef(rotX, 1.f, 0, 0);
    if (rotY != 0)
        glRotatef(rotY, 0, 1.f, 0);
    if (rotZ != 0)
        glRotatef(rotZ, 0, 0, 1.f);

    // Top of the hexagon
    glBegin(GL_POLYGON);
    glVertex3f(float(cos(11.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(11.f * M_PI / 6.f) * 0.5f));
    glVertex3f(0, 0.5f, -0.5f);
    glVertex3f(float(cos(7.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(7.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(5.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(5.f * M_PI / 6.f) * 0.5f));
    glVertex3f(0, 0.5f, 0.5f);
    glVertex3f(float(cos(M_PI / 6.f) * 0.5f), 0.5f, float(sin(M_PI / 6.f) * 0.5f));

    glEnd();

    // Bottom of the hexagon
    glBegin(GL_POLYGON);
    glVertex3f(0, -0.5f, 0.5f);
    glVertex3f(float(cos(5.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(5.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(7.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(7.f * M_PI / 6.f) * 0.5f));
    glVertex3f(0, -0.5f, -0.5f);
    glVertex3f(float(cos(11.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(11.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(M_PI / 6.f) * 0.5f), -0.5f, float(sin(M_PI / 6.f) * 0.5f));
    glEnd();

    // Connections
    glBegin(GL_QUADS);
    glVertex3f(float(cos(11.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(11.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(11.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(11.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(M_PI / 6.f) * 0.5f), 0.5f, float(sin(M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(M_PI / 6.f) * 0.5f), -0.5f, float(sin(M_PI / 6.f) * 0.5f));

    glVertex3f(float(cos(M_PI / 6.f) * 0.5f), -0.5f, float(sin(M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(M_PI / 6.f) * 0.5f), 0.5f, float(sin(M_PI / 6.f) * 0.5f));
    glVertex3f(0, 0.5f, 0.5f);
    glVertex3f(0, -0.5f, 0.5f);

    glVertex3f(0, -0.5f, 0.5f);
    glVertex3f(0, 0.5f, 0.5f);
    glVertex3f(float(cos(5.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(5.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(5.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(5.f * M_PI / 6.f) * 0.5f));

    glVertex3f(float(cos(5.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(5.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(5.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(5.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(7.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(7.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(7.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(7.f * M_PI / 6.f) * 0.5f));

    glVertex3f(float(cos(7.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(7.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(7.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(7.f * M_PI / 6.f) * 0.5f));
    glVertex3f(0, 0.5f, -0.5f);
    glVertex3f(0, -0.5f, -0.5f);

    glVertex3f(0, -0.5f, -0.5f);
    glVertex3f(0, 0.5f, -0.5f);
    glVertex3f(float(cos(11.f * M_PI / 6.f) * 0.5f), 0.5f, float(sin(11.f * M_PI / 6.f) * 0.5f));
    glVertex3f(float(cos(11.f * M_PI / 6.f) * 0.5f), -0.5f, float(sin(11.f * M_PI / 6.f) * 0.5f));
    glEnd();
    glPopMatrix();
}

void Engine::DrawCube(int x, int y, int z, float rotX, float rotY, float rotZ)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    if (rotX != 0)
        glRotatef(rotX, 1.f, 0, 0);
    if (rotY != 0)
        glRotatef(rotY, 0, 1.f, 0);
    if (rotZ != 0)
        glRotatef(rotZ, 0, 0, 1.f);
    m_textureAtlas.Bind();
    glBegin(GL_QUADS);
    // TOP
    glTexCoord2f(0, 0);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.f, 0);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0, 1.f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    glTexCoord2f(0, 0);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.f, 0);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0, 1.f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glTexCoord2f(0, 0);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.f, 0);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0, 1.f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    glTexCoord2f(0, 0);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.f, 0);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0, 1.f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    glTexCoord2f(0, 0);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.f, 0);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0, 1.f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // BOTTOM
    glTexCoord2f(0, 0);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.f, 0);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0, 1.f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    glPopMatrix();
}

KRYO_END_NAMESPACE
