//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef LAB03_NODE_H
#define LAB03_NODE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>

#include <map>
#include <vector>
enum cnt_dir{
    up = 0,
    down = 1,
    left = 2,
    right = 3,
    front =4,
    back = 5
};
class Node {
public:
    static GLuint counter;
    GLuint id;
    glm::vec3 pos;
    std::vector<std::vector<Node*> > connections;
    std::vector<GLuint> occupied_surfaces;
    Node();
    explicit Node(glm::vec3 p);
    void add_connection(cnt_dir dir, Node *n);
    std::vector<GLuint> unoccupied_dir();
    std::vector<GLuint> get_connections();
    void add_occupied_surfaces(GLuint );
    bool is_endpoint();
};

#endif //LAB03_NODE_H
