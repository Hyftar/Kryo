#include "engine.h"

KRYO_BEGIN_NAMESPACE

Engine::Engine() : m_wireframe(false),
m_moveForward(false), m_moveBackward(false),
m_moveLeft(false), m_moveRight(false),
m_moveUp(false), m_moveDown(false),
m_player(CHUNK_VIEW_DISTANCE_X * CHUNK_SIZE_WIDTH / 2.f, 100.f, CHUNK_VIEW_DISTANCE_Y * CHUNK_SIZE_DEPTH / 2.f, 90, 0, Vector3f(PLAYER_SPEED, 0.f, PLAYER_SPEED)),
/*m_chunks(CHUNK_VIEW_DISTANCE_X, CHUNK_VIEW_DISTANCE_Y, CHUNK_BUFFER_SIZE_WIDTH, CHUNK_BUFFER_SIZE_HEIGHT),*/
m_chunks(CHUNK_VIEW_DISTANCE_X, CHUNK_VIEW_DISTANCE_Y),
m_blockDefinitions(BLOCK_TYPE_MAX, BLOCK_TYPE_MAX),
m_textureAtlas(TEXTUREATLAS_SIZE),
m_fps(0) { }

Engine::~Engine() { }

void Engine::LoadChunks()
{
    for (int x = 0; x < CHUNK_VIEW_DISTANCE_X; x++)
    {
        for (int y = 0; y < CHUNK_VIEW_DISTANCE_Y; y++)
        {
            m_chunks.Set(x, y, new Chunk(this, x, y));
        }
    }
}

void Engine::Init()
{
    LoadChunks();
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Error while initializing GLEW... aborting (" << glewGetErrorString(err) << ")" << std::endl;
        abort();
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)Width() / (float)Height(), 0.001f, 1000.f);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_CULL_FACE);

    GLfloat fogColor[3] = { 0.8f, 0.8f, 0.8f };

    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.05f);

    // Light
    GLfloat light0Pos[4] = { 0.0f, CHUNK_SIZE_HEIGHT, 0.0f, 1.0f };
    GLfloat light0Amb[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat light0Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light0Spec[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

    glEnable(GL_LIGHTING);
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

BlockInfo::BlockUV* Engine::CreateBlockUV(
    TextureAtlas::TextureIndex frontIndex, TextureAtlas::TextureIndex backIndex,
    TextureAtlas::TextureIndex rightIndex, TextureAtlas::TextureIndex leftIndex,
    TextureAtlas::TextureIndex topIndex, TextureAtlas::TextureIndex bottomIndex) const
{
    BlockInfo::BlockUV* m = new BlockInfo::BlockUV();
    m_textureAtlas.TextureIndexToCoord(frontIndex, m->fu, m->fv, m->fw, m->fh);
    m_textureAtlas.TextureIndexToCoord(backIndex, m->du, m->dv, m->dw, m->dh);
    m_textureAtlas.TextureIndexToCoord(rightIndex, m->ru, m->rv, m->rw, m->rh);
    m_textureAtlas.TextureIndexToCoord(leftIndex, m->lu, m->lv, m->lw, m->lh);
    m_textureAtlas.TextureIndexToCoord(topIndex, m->tu, m->tv, m->tw, m->th);
    m_textureAtlas.TextureIndexToCoord(bottomIndex, m->bu, m->bv, m->bw, m->bh);
    return m;
}

void Engine::AddBlockDefinition(const BlockType bt, const std::string& name,
    const std::string& frontPath, const std::string& backPath,
    const std::string& rightPath, const std::string& leftPath,
    const std::string& topPath, const std::string& bottomPath,
    const bool solid)
{
    TextureAtlas::TextureIndex
        frontIndex = m_textureAtlas.AddTexture(frontPath),
        backIndex = m_textureAtlas.AddTexture(backPath),
        rightIndex = m_textureAtlas.AddTexture(rightPath),
        leftIndex = m_textureAtlas.AddTexture(leftPath),
        topIndex = m_textureAtlas.AddTexture(topPath),
        bottomIndex = m_textureAtlas.AddTexture(bottomPath);

    BlockInfo::BlockUV* m = CreateBlockUV(frontIndex, backIndex, rightIndex, leftIndex, topIndex, bottomIndex);

    m_blockDefinitions.Set(bt, new BlockInfo(bt, name, *m, solid));
}

void Engine::AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& path, const bool solid)
{
    return AddBlockDefinition(bt, name, path, path, path, path, path, path, solid);
}

void Engine::AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& topPath, const std::string& sidePath, const bool solid)
{
    return AddBlockDefinition(bt, name, sidePath, sidePath, sidePath, sidePath, topPath, sidePath, solid);
}

void Engine::AddBlockDefinition(const BlockType bt, const std::string& name, const std::string& topPath, const std::string& frontPath, const std::string& sidePath, const bool solid)
{
    return AddBlockDefinition(bt, name, frontPath, sidePath, sidePath, sidePath, topPath, sidePath, solid);
}

void Engine::LoadResource()
{
    m_textureFont.Load(TEXTURE_PATH "font.bmp");
    m_textureCrosshair.Load(TEXTURE_PATH "cross.bmp");

    AddBlockDefinition(BTYPE_DIRT, "Dirt", TEXTURE_PATH "dirt.png");
    AddBlockDefinition(BTYPE_GRASS, "Grass",
        TEXTURE_PATH "grass_side.png",
        TEXTURE_PATH "grass_side.png",
        TEXTURE_PATH "grass_side.png",
        TEXTURE_PATH "grass_side.png",
        TEXTURE_PATH "grass.png",
        TEXTURE_PATH "dirt.png");
    AddBlockDefinition(BTYPE_STONE, "Stone", TEXTURE_PATH "stone.png");

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
    m_fps = 1.f / elapsedTime;

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

    m_shader01.Use();

    GLint fogMode;
    glGetIntegerv(GL_FOG_MODE, &fogMode);
    glUniform1i(glGetUniformLocationARB(m_shader01.m_program, "fogMode"), fogMode);

    for (int x = 0; x < CHUNK_VIEW_DISTANCE_X; x++)
    {
        for (int y = 0; y < CHUNK_VIEW_DISTANCE_Y; y++)
        {
            Chunk* chunk = m_chunks.Get(x, y);

            assert(chunk != nullptr);

            if (chunk->IsDirty())
                chunk->Update(CHUNK_SIZE_WIDTH * x, CHUNK_SIZE_DEPTH * y, x, y);
                //chunk->Update((CHUNK_SIZE_WIDTH * x) - (CHUNK_SIZE_WIDTH * CHUNK_VIEW_DISTANCE_X / 2), (CHUNK_SIZE_DEPTH * y) - (CHUNK_SIZE_DEPTH * CHUNK_VIEW_DISTANCE_Y / 2), x, y);
            chunk->Render();
        }
    }

    Shader::Disable();

    if (m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    DrawHud();
    if (m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
            m_player.SetSpeedY(5.f);
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

BlockType Engine::GetBlock_s(int chunkX, int chunkY, int nx, int ny, int nz) const
{
    if (ny < 0 || ny >= CHUNK_SIZE_HEIGHT)
        return BTYPE_AIR;

    chunkX += floor(nx / float(CHUNK_SIZE_WIDTH));
    int x = (nx < 0 ? CHUNK_SIZE_WIDTH + nx : nx) % CHUNK_SIZE_WIDTH;

    chunkY += floor(nz / float(CHUNK_SIZE_DEPTH));
    int z = (nz < 0 ? CHUNK_SIZE_DEPTH + nz : nz) % CHUNK_SIZE_DEPTH;

    if (chunkX < 0 || chunkY < 0 || chunkX >= CHUNK_VIEW_DISTANCE_X || chunkY >= CHUNK_VIEW_DISTANCE_Y)
        return BTYPE_AIR;

    Chunk* chunk = m_chunks.Get(chunkX, chunkY);

    if (chunk == nullptr)
        return BTYPE_AIR;

    return chunk->Get(x, ny, z);
}

BlockType Engine::GetBlock_s(int x, int y, int z) const
{
    return GetBlock_s(0, 0, x, y, z);
}

BlockType Engine::GetBlock_s(int chunkX, int chunkY, Vector3i v) const
{
    return GetBlock_s(chunkX, chunkY, v.x, v.y, v.z);
}

BlockType Engine::GetBlock_s(Vector3i v) const
{
    return GetBlock_s(v.x, v.y, v.z);
}

/*BlockType Engine::GetBlock_s(int chunkX, int chunkY, int x, int y, int z) const
{
    return m_chunks.GetBlock_s(chunkX, chunkY, x, y, z);
}*/

void Engine::GetBlockAtCursor()
{
    int x = Width() / 2;
    int y = Height() / 2;

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    //posX += .5f;
    //posY += .5f;
    //posZ += .5f;

    // Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
    // de valeurs negatives
    int px = (int)(posX);
    int py = (int)(posY);
    int pz = (int)(posZ);

    bool found = false;

    if ((m_player.GetPosition() - Vector3f(posX, posY, posZ)).Length() < MAX_SELECTION_DISTANCE)
    {
        // Apres avoir determine la position du bloc en utilisant la partie entiere du hit
        // point retourne par opengl, on doit verifier de chaque cote du bloc trouve pour trouver
        // le vrai bloc. Le vrai bloc peut etre different a cause d'erreurs de precision de nos
        // nombres flottants (si z = 14.999 par exemple, et qu'il n'y a pas de blocs a la position
        // 14 (apres arrondi vers l'entier) on doit trouver et retourner le bloc en position 15 s'il existe
        // A cause des erreurs de precisions, ils arrive que le cote d'un bloc qui doit pourtant etre a la
        // position 15 par exemple nous retourne plutot la position 15.0001
        for (int x = px - 1; !found && x <= px + 1; ++x)
        {
            for (int y = py - 1; !found && x >= 0 && y <= py + 1; ++y)
            {
                for (int z = pz - 1; !found && y >= 0 && z <= pz + 1; ++z)
                {
                    if (z >= 0)
                    {
                        BlockType bt = GetBlock_s(x, y, z);
                        if (bt == BTYPE_AIR)
                            continue;

                        m_currentBlock.x = x;
                        m_currentBlock.y = y;
                        m_currentBlock.z = z;

                        if (Tool::InRangeWithEpsilon<float>(posX, x, x + 1, 0.05) && Tool::InRangeWithEpsilon<float>(posY, y, y + 1, 0.05) && Tool::InRangeWithEpsilon<float>(posZ, z, z + 1, 0.05))
                        {
                            found = true;
                        }
                    }
                }
            }
        }
    }

    if (!found)
    {
        m_currentBlock = -1;
    }
    else
    {
        // Find on which face of the block we got a hit
        m_currentFaceNormal.Zero();

        // Front et back:
        if (Tool::EqualWithEpsilon<float>(posZ, m_currentBlock.z, 0.005f))
            m_currentFaceNormal.z = -1;
        else if (Tool::EqualWithEpsilon<float>(posZ, m_currentBlock.z + 1, 0.005f))
            m_currentFaceNormal.z = 1;
        else if (Tool::EqualWithEpsilon<float>(posX, m_currentBlock.x, 0.005f))
            m_currentFaceNormal.x = -1;
        else if (Tool::EqualWithEpsilon<float>(posX, m_currentBlock.x + 1, 0.005f))
            m_currentFaceNormal.x = 1;
        else if (Tool::EqualWithEpsilon<float>(posY, m_currentBlock.y, 0.005f))
            m_currentFaceNormal.y = -1;
        else if (Tool::EqualWithEpsilon<float>(posY, m_currentBlock.y + 1, 0.005f))
            m_currentFaceNormal.y = 1;
    }
}

int Engine::GetFps() const
{
    return m_fps;
}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t) const
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
    // Setter le blend function , tout ce qui sera noir sera transparent
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho(0, Width(), 0, Height(), -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            // Bind de la texture pour le font
            m_textureFont.Bind();
            std::ostringstream ss;
            ss << "FPS: " << GetFps();
            PrintText(10, Height() - 25, ss.str());
            ss.str("");
            // TODO: remplacer par un Vector2i
            Vector2<int> chunkPos = m_player.GetChunkPosition();
            ss << "Chunk: " << "[" << chunkPos.x << ", " << chunkPos.y << "]";
            PrintText(10, 25, ss.str());
            ss.str("");
            ss << "Position: " << m_player.GetPosition();
            PrintText(10, 10, ss.str());

            // Affichage du crosshair
            m_textureCrosshair.Bind();
            static const int crossSize = 32;
            glLoadIdentity();
            glTranslated(Width() / 2 - crossSize / 2, Height() / 2 - crossSize / 2, 0);
            glBegin(GL_QUADS);
                glTexCoord2f(0, 0);
                glVertex2i(0, 0);
                glTexCoord2f(1, 0);
                glVertex2i(crossSize, 0);
                glTexCoord2f(1, 1);
                glVertex2i(crossSize, crossSize);
                glTexCoord2f(0, 1);
                glVertex2i(0, crossSize);
            glEnd();
            glEnable(GL_LIGHTING);
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Engine::CheckCollisions(Player& player, Vector3f movement)
{
    Vector3f playerPos = player.GetPosition();
    Vector2<int> chunkPos = player.GetChunkPosition();
    Vector3f expectedPos = playerPos + movement;

    auto aposY = playerPos.y;
    auto bposY = playerPos.y + (PLAYER_HEIGHT / 2.f);
    auto cposY = playerPos.y + PLAYER_HEIGHT;
    if (GetBlock_s(int(expectedPos.x - BLOCK_MARGIN), aposY, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x + BLOCK_MARGIN), aposY, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x + BLOCK_MARGIN), aposY, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x - BLOCK_MARGIN), aposY, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||

        GetBlock_s(int(expectedPos.x - BLOCK_MARGIN), bposY, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x + BLOCK_MARGIN), bposY, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x + BLOCK_MARGIN), bposY, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x - BLOCK_MARGIN), bposY, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||

        GetBlock_s(int(expectedPos.x - BLOCK_MARGIN), cposY, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x + BLOCK_MARGIN), cposY, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x + BLOCK_MARGIN), cposY, int(playerPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(expectedPos.x - BLOCK_MARGIN), cposY, int(playerPos.z + BLOCK_MARGIN)) != BTYPE_AIR)
    {
        movement.x = 0;
    }
    playerPos.x += movement.x;

    if (GetBlock_s(int(playerPos.x - BLOCK_MARGIN), aposY, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x + BLOCK_MARGIN), aposY, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x + BLOCK_MARGIN), aposY, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x - BLOCK_MARGIN), aposY, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||

        GetBlock_s(int(playerPos.x - BLOCK_MARGIN), bposY, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x + BLOCK_MARGIN), bposY, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x + BLOCK_MARGIN), bposY, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x - BLOCK_MARGIN), bposY, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||

        GetBlock_s(int(playerPos.x - BLOCK_MARGIN), cposY, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x + BLOCK_MARGIN), cposY, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x + BLOCK_MARGIN), cposY, int(expectedPos.z - BLOCK_MARGIN)) != BTYPE_AIR ||
        GetBlock_s(int(playerPos.x - BLOCK_MARGIN), cposY, int(expectedPos.z + BLOCK_MARGIN)) != BTYPE_AIR)
    {
        movement.z = 0;
    }
    playerPos.z += movement.z;

    {
        int blockPlayerY = int(playerPos.y);
        bool positive = blockPlayerY <= expectedPos.y;
        int delta = abs(blockPlayerY - expectedPos.y);
        if (playerPos != expectedPos && blockPlayerY >= 0)
        {
            //std::cout << playerPos.y << std::endl;
            int i = delta;

            do
            {
                float playerY = positive ? playerPos.y + i : blockPlayerY - (delta - i);
                float blockY = positive ? int(playerY) + BLOCK_HEIGHT + BLOCK_HEIGHT : int(playerY)  - BLOCK_HEIGHT;

                auto btL = GetBlock_s(int(playerPos.x + BLOCK_MARGIN), blockY, int(playerPos.z));
                auto btR = GetBlock_s(int(playerPos.x - BLOCK_MARGIN), blockY, int(playerPos.z));
                auto btF = GetBlock_s(int(playerPos.x), blockY, int(playerPos.z + BLOCK_MARGIN));
                auto btB = GetBlock_s(int(playerPos.x), blockY, int(playerPos.z - BLOCK_MARGIN));

                auto btD1 = GetBlock_s(int(playerPos.x - BLOCK_MARGIN), blockY, int(playerPos.z - BLOCK_MARGIN));
                auto btD2 = GetBlock_s(int(playerPos.x - BLOCK_MARGIN), blockY, int(playerPos.z + BLOCK_MARGIN));
                auto btD3 = GetBlock_s(int(playerPos.x + BLOCK_MARGIN), blockY, int(playerPos.z - BLOCK_MARGIN));
                auto btD4 = GetBlock_s(int(playerPos.x + BLOCK_MARGIN), blockY, int(playerPos.z + BLOCK_MARGIN));

                if ((btL != BTYPE_AIR || btR != BTYPE_AIR || btF != BTYPE_AIR || btB != BTYPE_AIR
                    || btD1 != BTYPE_AIR || btD2 != BTYPE_AIR || btD3 != BTYPE_AIR || btD4 != BTYPE_AIR))
                {
                    playerPos.y = int(playerY);
                    if (!player.IsFreecam())
                        player.SetSpeedY(0);
                    goto endCheckY;
                }
            } while (i--);
            playerPos.y += movement.y;
        }
    }
endCheckY:

    player.SetPosition(playerPos);

    #undef KRYO_HITBOX_COMPARE
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
