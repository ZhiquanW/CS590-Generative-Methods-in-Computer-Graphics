//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef HELLO_WORLD_VERTEXARRAYOBJECT_H
#define HELLO_WORLD_VERTEXARRAYOBJECT_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class VertexArrayObject {
private:
    GLuint vao_id;
    std::vector<GLuint> vbo_list;
    std::vector<GLuint> ebo_list;
public:
    VertexArrayObject();

    explicit VertexArrayObject(bool);

    GLuint id();

    void activate();

    void attach_vbo(GLuint);

    void attach_ebo(GLuint);

    void release();

};


#endif //HELLO_WORLD_VERTEXARRAYOBJECT_H
