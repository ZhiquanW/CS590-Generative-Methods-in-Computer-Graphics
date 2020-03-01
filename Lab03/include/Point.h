//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef LAB03_POINT_H
#define LAB03_POINT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
class Point {
public:
    static  GLuint counter;
    GLuint id;
    glm::vec3 pos{};
    Point();
    Point(glm::vec3);
};


#endif //LAB03_POINT_H
