#pragma once

#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

MeshData Renderer::uploadMesh(std::vector<Vertex>& mesh, std::vector<int> elem) {
    MeshData data;

    glGenVertexArrays(1, &data.VAO);
    glBindVertexArray(data.VAO);

    glGenBuffers(1, &data.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), mesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Pos));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &data.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elem.size() * sizeof(uint32_t), elem.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return data;
}

void Renderer::unloadMesh(MeshData& mesh) {

    
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    
}

void Renderer::drawMesh(MeshData& mesh, uint32_t numIndices) {
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

