#ifndef KRYO_GLOBAL_H
#define KRYO_GLOBAL_H

#include "system.h"
#include <assert.h>

#include <stdint.h>

static_assert(std::numeric_limits<float>::is_iec559 && std::numeric_limits<double>::is_iec559, "The floating point value types must conform to the IEC 559/IEEE 754 standards");
static_assert(sizeof(uint8_t) == 1, "The uint8 value type must be of exactly 1 byte");
static_assert(sizeof(uint16_t) == 2, "The uint16 value type must be of exactly 2 bytes");
static_assert(sizeof(uint32_t) == 4, "The uint32 value type must be of exactly 4 bytes");
static_assert(sizeof(uint64_t) == 8, "The uint64 value type must be of exactly 8 bytes");
static_assert(sizeof(int8_t) == 1, "The int8 value type must be of exactly 1 byte");
static_assert(sizeof(int16_t) == 2, "The int16 value type must be of exactly 2 bytes");
static_assert(sizeof(int32_t) == 4, "The int32 value type must be of exactly 4 bytes");
static_assert(sizeof(int64_t) == 8, "The int64 value type must be of exactly 8 bytes");

// GLEW
#include <gl/glew.h>

// SFML
#include <SFML/Window.hpp>

// DevIL
#include <IL/il.h>

// OpenGL
#include <gl/GL.h>
#include <gl/GLU.h>

#define KRYO_BEGIN_NAMESPACE namespace Kryo {
#define KRYO_END_NAMESPACE }

#endif
