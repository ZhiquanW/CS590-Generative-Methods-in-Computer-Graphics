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
#include "Point.h"
class Surface {
public:
    static GLuint counter;
    GLuint id;
    glm::vec4 indices;
    glm::vec3 center_pt;
    Surface();
    Surface(glm::vec4);
    bool is_contain_point(GLuint);
    void set_center_pt(glm::vec3);
    void set_edge_pts(std::vector<Point>);
    glm::vec2 get_edges_by_p(GLuint);
    bool contain_edge(std::vector<GLuint>);
};


#endif //LAB03_SURFACE_H
