#include "Mesh.h"

Mesh::Mesh() // Constructor
{
    m_vao = 0;
    m_vbo = 0;
    m_ebo = 0;
    m_indexCount = 0;
}

void Mesh::CreateMesh(GLfloat *a_tPVertices, unsigned int *a_tPIndices, unsigned int a_tNumberOfVertices,
    unsigned int a_tNumberOfIndices)
{
    m_indexCount = a_tNumberOfIndices;

    glGenVertexArrays(1, &m_vao); // Generate VAO
    glBindVertexArray(m_vao);     // Bind VAO

    glGenBuffers(1, &m_ebo); // Generate IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo); // Bind EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(a_tPIndices[0]) * a_tNumberOfIndices, a_tPIndices, GL_STATIC_DRAW); // Copy indices to EBO

    glGenBuffers(1, &m_vbo); // Generate VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(a_tPVertices[0]) * a_tNumberOfVertices, a_tPVertices, GL_STATIC_DRAW); // Copy vertices to VBO

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Specify layout of VBO
    glEnableVertexAttribArray(0); // Enable vertex attribute array 0

    glBindVertexArray(0); // Unbind VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
}

void Mesh::RenderMesh()
{
    glBindVertexArray(m_vao); // Bind VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo); // Bind EBO
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0); // Draw mesh
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO
    glBindVertexArray(0); // Unbind VAO
}

void Mesh::ClearMesh()
{
    if (m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo); // Delete EBO
        m_ebo = 0;
    }

    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo); // Delete VBO
        m_vbo = 0;
    }

    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao); // Delete VAO
        m_vao = 0;
    }

    m_indexCount = 0;
}

Mesh::~Mesh() // Destructor
{
    ClearMesh();
}
