#include "textureatlas.h"
#include "tool.h"

KRYO_BEGIN_NAMESPACE

TextureAtlas::TextureAtlas(unsigned int nbTexture) : m_currentTextureIndex(0), m_isValid(false)
{
    if(nbTexture < 4)
        nbTexture = 4;

    // Arrondir sur la puissance de 2 superieure
    m_nbTexturePerSide = (int)sqrt((float)nbTexture);
    if (m_nbTexturePerSide * m_nbTexturePerSide < nbTexture)
        m_nbTexturePerSide++;
    while (!IsPowerOfTwo(m_nbTexturePerSide))
        m_nbTexturePerSide++;
}

TextureAtlas::~TextureAtlas()
{
    if(IsValid())
        glDeleteTextures(1, &m_textureId);
}

TextureAtlas::TextureIndex TextureAtlas::AddTexture(const std::string& fname)
{
    TextureList::iterator it = m_textureList.find(fname);

    if(it != m_textureList.end())
        return it->second.texIdx;

    TextureIndex id = m_currentTextureIndex++;
    m_textureList.insert(std::make_pair(fname, TextureInfo((ILuint)-1, id)));
    return id;
}

bool TextureAtlas::Generate(int textureSize, bool mipmap)
{
    // TODO mipmap pas encore 100% parfait...
    assert(!mipmap);

    if(!IsPowerOfTwo(textureSize))
        return false;

    // Initialize DevIL only once:
    static bool alreadyInitialized = false;
    if(!alreadyInitialized)
    {
        ilInit();
        iluInit();
        alreadyInitialized = true;
    }

    for(TextureList::iterator it = m_textureList.begin(); it != m_textureList.end(); ++it)
    {
        ILuint texid = it->second.texId;
        if(texid == (ILuint)-1)
        {
            std::cout << "Loading " << it->first << " (id=" << it->second.texIdx << ")..." << std::endl;
            ilGenImages(1, &texid);
            ilBindImage(texid);

            ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
            ilEnable(IL_ORIGIN_SET);

            if (!ilLoadImage((const ILstring)it->first.c_str()))
                return false;

            if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
                return false;

            iluScale(textureSize, textureSize, 1);

            it->second.texId = texid;
        }
    }

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    if(mipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST_MIPMAP_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    int level = textureSize;
    int oglLevel = 0;
    int mipmapSize = textureSize * m_nbTexturePerSide;
    while (mipmapSize != 0)
    {
        ILuint atlasTex;
        ilGenImages(1, &atlasTex);
        ilBindImage(atlasTex);
        ilTexImage(mipmapSize, mipmapSize, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, 0);
        ilClearColour(1, 0, 0, 1);
        ilClearImage();

        for (TextureList::iterator it = m_textureList.begin(); it != m_textureList.end(); ++it)
        {
            ILuint tmpImg;
            ilGenImages(1, &tmpImg);
            ilBindImage(tmpImg);
            ilCopyImage(it->second.texId);

            iluImageParameter(ILU_FILTER, ILU_NEAREST);
            if(level != textureSize)
                iluScale(level, level, 1);

            char* data = new char[level * level * 4];
            ilCopyPixels(0, 0, 0, level, level, 1, IL_RGBA, IL_UNSIGNED_BYTE, data);


            int imgIdx = it->second.texIdx;
            int x = imgIdx % m_nbTexturePerSide;
            int y = m_nbTexturePerSide - 1 - imgIdx / m_nbTexturePerSide;
            ilBindImage(atlasTex);
            ilSetPixels(x * level, y * level, 0, level, level, 1, IL_RGBA, IL_UNSIGNED_BYTE, data);

            delete [] data;
            ilDeleteImages(1, &tmpImg);
        }

        // TODO
        //if(level == textureSize)
        //{
        //ilEnable(IL_FILE_OVERWRITE);
        //ilSaveImage("textureatlas.png");
        //}

        glTexImage2D(GL_TEXTURE_2D, oglLevel++, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

        CHECK_GL_ERROR();


        ilDeleteImages(1, &atlasTex);

        if(!mipmap)
            break;

        level /= 2;
        mipmapSize /= 2;
    }

    m_isValid = true;
    return true;
}


bool TextureAtlas::IsValid() const
{
    return m_isValid;
}

void TextureAtlas::Bind() const
{
    assert(IsValid());
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void TextureAtlas::TextureIndexToCoord(TextureIndex idx, float& u, float& v, float& w, float& h) const
{
    w = 1.f / (float)m_nbTexturePerSide;
    h = 1.f / (float)m_nbTexturePerSide;

    u = (float)((unsigned int)idx % m_nbTexturePerSide) * w;
    v = (float)(m_nbTexturePerSide - 1 - (unsigned int)idx / m_nbTexturePerSide) * h;
}

KRYO_END_NAMESPACE
