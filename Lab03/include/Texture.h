//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//
#ifndef HELLO_WORLD_TEXTURE_H
#define HELLO_WORLD_TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
private:
    GLuint texture_id;
    GLuint index;
    std::string type;
    static GLuint texture_counter;
public :
    Texture();

    explicit Texture(GLuint, GLenum= GL_REPEAT, GLenum= GL_LINEAR);

    bool load_image(const char *);

    void activate();

    void release();
};


#endif //HELLO_WORLD_TEXTURE_H
