#ifndef KRYO_TOOL_H
#define KRYO_TOOL_H

#include "global.h"

KRYO_BEGIN_NAMESPACE

// TODO: ne pas oublier de ne pas definir DEBUGMODE en release
#ifndef DEBUGMODE
#define DEBUGMODE
#endif

#ifdef DEBUGMODE
#	define CHECK_GL_ERROR() Tool::CheckGLError(__FILE__, __LINE__);
#else
#	define CHECK_GL_ERROR()
#endif


class Tool
{
public:
    static bool LoadTextFile(const std::string& filename, std::string& buffer);
    static void CheckGLError(const char* file, int line);
};

KRYO_END_NAMESPACE

#endif
