#include "engine.h"

KRYO_BEGIN_NAMESPACE

Engine::Engine()
    : m_wireframe(false), m_moveUp(false), m_moveDown(false), m_moveLeft(false), m_moveRight(false) { }

Engine::~Engine() { }

void Engine::Init()
{
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

void Engine::LoadResource()
{
    LoadTexture(m_textureFloor, TEXTURE_PATH "checker.bmp");
    LoadTexture(m_textureCube, TEXTURE_PATH "tile.jpg");
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
    m_player.Move(m_moveUp, m_moveDown, m_moveLeft, m_moveRight, elapsedTime);
    m_player.ApplyRotation();
    m_player.ApplyTranslation();

    // Plancher
    // Les vertex doivent etre affiches dans le sens anti-horaire (CCW)
    m_textureFloor.Bind();
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
    DrawHexagon(0, -1, -7);
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
    case 0: // A
        m_moveLeft = true;
        break;
    case 22: // W
        m_moveUp = true;
        break;
    case 18: // S
        m_moveDown = true;
        break;
    case 3: // D
        m_moveRight = true;
        break;
    case 24: // Y
        m_wireframe = !m_wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, (m_wireframe ? GL_LINE : GL_FILL));
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
        m_moveUp = false;
        break;
    case 18: // S
        m_moveDown = false;
        break;
    case 3: // D
        m_moveRight = false;
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
    if (x == (Width() / 2) && y == (Height() / 2))
        return;
    CenterMouse();
    MakeRelativeToCenter(x, y);
    if (x != (Width() / 2))
        m_player.TurnLeftRight(x);

    if (y != (Height() / 2))
        m_player.TurnTopBottom(y);
}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
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
        m_textureCube.Bind();
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
