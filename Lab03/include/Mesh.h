//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//
#ifndef HELLO_WORLD_VERTEX_H
#define HELLO_WORLD_VERTEX_H

#include "Texture.h"

#include <vector>
#include <glm/vec3.hpp>
#include "assimp/scene.h"

class Vertex {
private:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tex_coord;
public:
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    GLuint vao, vbo, ebo;

    void setup();

public:
    Mesh(std::vector<Vertex>, std::vector<GLuint>, std::vector<Texture>);

    void render();
};


#endif //HELLO_WORLD_VERTEX_H
