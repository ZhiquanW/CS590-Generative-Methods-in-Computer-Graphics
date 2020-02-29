//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
///*
//
//    glGenTextures(1, &this->texture1);
//    glBindTexture(GL_TEXTURE_2D, this->texture1);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //when minify, linear filter
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //when magnify, linear filter
//    glUniform1i(2 ,1);

// */
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

GLuint Texture::texture_counter = 0;

Texture::Texture() : texture_id(0), index(Texture::texture_counter++) {
    glGenTextures(1, &this->texture_id);

}

Texture::Texture(GLuint loc, GLenum warp_type, GLenum filter_type) : texture_id(0), index(Texture::texture_counter++) {
    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    glUniform1i(loc, this->index);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_type); //when minify, linear filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_type); //when magnify, linear filter
}

bool Texture::load_image(const char *file_name) {
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        return false;
    }
    stbi_image_free(data);
    return true;
}

void Texture::activate() {
    glActiveTexture(GL_TEXTURE0 + this->index);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
}

void Texture::release() {
    glDeleteTextures(1, &this->texture_id);
}