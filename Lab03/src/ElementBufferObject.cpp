//
// @Author: Zhiquan Wang
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//
#include "ElementBufferObject.h"
#include <vector>

ElementBufferObject::ElementBufferObject() : ebo_id(0) {
}

ElementBufferObject::ElementBufferObject(std::vector<GLuint> &indices, GLenum data_type) : ebo_id(0) {
    glGenBuffers(1, &this->ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], data_type);
}

GLuint ElementBufferObject::id() {
    return this->ebo_id;
}