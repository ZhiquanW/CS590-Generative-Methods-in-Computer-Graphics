//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include "VertexArrayObject.h"
#include <iostream>

VertexArrayObject::VertexArrayObject() : vao_id(0) {
    glGenVertexArrays(1, &this->vao_id);
}

VertexArrayObject::VertexArrayObject(bool activate) : vao_id(0) {
    glGenVertexArrays(1, &this->vao_id);
    if (activate) {
        this->activate();
    }
}

void VertexArrayObject::activate() {
    glBindVertexArray(this->vao_id);
}

GLuint VertexArrayObject::id() {
    return this->vao_id;
}

void VertexArrayObject::attach_vbo(GLuint vbo_id) {
    this->vbo_list.push_back(vbo_id);
}

void VertexArrayObject::attach_ebo(GLuint veo_id) {
    this->ebo_list.push_back(veo_id);
}

void VertexArrayObject::release() {
    //release vbo
    std::vector<GLuint>::iterator itor;
    for (itor = this->vbo_list.begin(); itor != this->vbo_list.end(); ++itor) {
        glDeleteBuffers(1, &(*itor));
    }
    //release ebo
    for (itor = this->ebo_list.begin(); itor != this->ebo_list.end(); ++itor) {
        glDeleteBuffers(1, &(*itor));
    }
    //release vao
    glDeleteVertexArrays(1, &this->vao_id);
}
//void VertexArrayObject::add_vertex_buffer_object(std::vector<glm::vec3> data,
//                                                 GLuint index,
//                                                 GLsizei size,
//                                                 const void *offset) {
//    //data, location,size,stride,offset
//    // unsigned int vbo_id;
//    // glGenBuffers(1, &vbo_id);
//    // glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
//    // glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
//}
