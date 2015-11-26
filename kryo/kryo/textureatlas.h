#ifndef KRYO_TEXTUREATLAS_H
#define KRYO_TEXTUREATLAS_H

#include "global.h"
#include <map>

KRYO_BEGIN_NAMESPACE

class TextureAtlas
{
public:
    typedef unsigned int TextureIndex;

public:
    TextureAtlas(unsigned int nbTexture);
    ~TextureAtlas();
    TextureIndex AddTexture(const std::string& fname);
    bool Generate(int textureSize, bool mipmap);

    bool IsValid() const;
    void Bind() const;

    void TextureIndexToCoord(TextureIndex idx, float& u, float& v, float& w, float& h) const;

private:
    bool IsPowerOfTwo(unsigned int x)
    {
        return ((x != 0) && ((x & (~x + 1)) == x));
    }

private:
    struct TextureInfo
    {
        ILuint texId;
        TextureIndex texIdx;

        TextureInfo(ILuint ntexId, unsigned int ntexIdx) : texId(ntexId), texIdx(ntexIdx) {}
    };
    // On utilise un std::map pour avoir des valeurs uniques
    typedef std::map<std::string, TextureInfo> TextureList;
    TextureList m_textureList;

    TextureIndex m_currentTextureIndex;
    GLuint  m_textureId;
    bool    m_isValid;
    unsigned int m_nbTexturePerSide;

};

KRYO_END_NAMESPACE

#endif
