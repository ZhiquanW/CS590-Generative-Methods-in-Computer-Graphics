//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include "VertexBufferObject.h"

VertexAttribute::VertexAttribute()
        : index(0), size(0), type(GL_FLOAT), normalized(GL_FALSE), stride(0), pointer(nullptr) {

}

VertexAttribute::VertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                 const void *pointer)
        : index(index), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer) {
}

void VertexAttribute::activate() {
    glVertexAttribPointer(this->index, this->size, this->type, this->normalized, this->stride, this->pointer);
    glEnableVertexAttribArray(this->index);
}


VertexBufferObject::VertexBufferObject(const std::vector<GLfloat> &data, GLenum data_type = GL_STATIC_DRAW) : vbo_id(0) {
    glGenBuffers(1, &this->vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], data_type);
}

VertexBufferObject::VertexBufferObject(const std::vector<glm::vec3> &data, GLenum data_type = GL_STATIC_DRAW) : vbo_id(0) {
    glGenBuffers(1, &this->vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * 3*sizeof(GLfloat), &data[0], data_type);
}

GLuint VertexBufferObject::id() {
    return this->vbo_id;
}

