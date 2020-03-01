//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef LAB03_TREE_H
#define LAB03_TREE_H
#include "Node.h"
#include "Surface.h"
#include "Point.h"

class Tree {
public:
    std::vector<Node> nodes;
    std::vector<Surface> surfaces;
    std::vector<Point> points;
    Tree();
    bool add_node(GLuint id,cnt_dir dir,Node &n);
    std::vector<glm::vec3> generate_points();
    std::vector<GLuint> generate_connections();
    void generate_surfaces();
    std::vector<glm::vec3> get_point_pos();
    std::vector<GLuint> get_surface_idx();
};


#endif //LAB03_TREE_H
