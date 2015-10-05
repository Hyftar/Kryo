#ifndef KRYO_TEXTURE_H
#define KRYO_TEXTURE_H

#include "global.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

class Texture
{
public:
    Texture(const std::string& filename = "");
    ~Texture();

    bool Load(const std::string& filename);
    bool IsValid() const;
    void Bind() const;

private:
    GLuint m_textureId;
    bool m_isValid;
};

KRYO_END_NAMESPACE

#endif
