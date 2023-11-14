#pragma once

#include <GL\glew.h>

class Mesh
{
public:
    Mesh();

    void CreateMesh(GLfloat* a_pVertices, unsigned int* a_pIndices, unsigned int a_numOfVertices, unsigned int a_numOfIndices);
    void RenderMesh();
    void ClearMesh();

    ~Mesh();

private:
    GLuint m_vao, m_vbo, m_ibo;
    GLsizei m_indexCount;
};

