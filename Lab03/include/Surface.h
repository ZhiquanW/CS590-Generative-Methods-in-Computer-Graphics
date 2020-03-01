//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef LAB03_SURFACE_H
#define LAB03_SURFACE_H


#include <vector>
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
class Surface {
public:
    static GLuint counter;
    GLuint id;
    glm::vec4 indices;
    Surface();
    Surface(glm::vec4);
};


#endif //LAB03_SURFACE_H
