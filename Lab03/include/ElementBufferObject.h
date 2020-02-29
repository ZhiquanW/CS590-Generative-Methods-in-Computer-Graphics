//
// @Author: Zhiquan Wang
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef HELLO_WORLD_ELEMENTBUFFEROBJECT_H
#define HELLO_WORLD_ELEMENTBUFFEROBJECT_H
#include <glad/glad.h>
#include <vector>
class ElementBufferObject {
private:
    GLuint ebo_id;
public:
    ElementBufferObject();
    ElementBufferObject(std::vector<GLuint> &,GLenum);
    GLuint id();
};


#endif //HELLO_WORLD_ELEMENTBUFFEROBJECT_H
