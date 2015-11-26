#ifndef KRYO_CHUNKMESH_H
#define KRYO_CHUNKMESH_H

#include "global.h"

KRYO_BEGIN_NAMESPACE

class ChunkMesh
{
public:
    // Structure représentant toutes les informations de chacuns des vertex
    // S'assurer que le size de cette struct reste un multiple de 32
    // octet pour un maximum de performance
    // Au besoin, ajouter du padding
    struct VertexData
    {
        float x, y, z;
        float r, g, b;
        float u, v;

        VertexData() { }
        VertexData(float nx, float ny, float nz, float nr, float ng, float nb, float nu, float nv)
            : x(nx), y(ny), z(nz), r(nr), g(ng), b(nb), u(nu), v(nv) {}
    };

public:
    ChunkMesh();
    ~ChunkMesh();

    bool IsValid() const;
    void SetMeshData(VertexData* vd, int vertexCount);
    void Render() const;

    int Count() const;

private:
    bool m_isValid;
    int m_vertexCount;
    GLuint m_vertexVboId;
    GLuint m_indexVboId;
};

KRYO_END_NAMESPACE

#endif
