#pragma once

#include <GL/glew.h>

class Mesh
{
public:
    Mesh();

    void CreateMesh(GLfloat *a_tPVertices, unsigned int *a_tPIndices, unsigned int a_tNumberOfVertices, unsigned int a_tNumberOfIndices); // Create a triangle
    void RenderMesh(); // Draw the triangle
    void ClearMesh(); // Delete the triangle
    
    ~Mesh();

private:
    GLuint m_vao, m_vbo, m_ebo;
    GLsizei m_indexCount;
};
