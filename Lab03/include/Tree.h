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
#include <cmath>
#include <algorithm>
#include <fstream>

class Tree {
public:
    std::vector<Node> nodes; //nodes on tree branch
    std::vector<Surface> surfaces; //tree surfaces
    std::vector<Point> points; //points on surfaces
    float offset;
    float height;
    Tree();
    bool add_node(GLuint id,cnt_dir dir,Node n);
    std::vector<glm::vec3> generate_points();
    std::vector<GLuint> generate_connections();
    void generate_surfaces();
    std::vector<glm::vec3> get_point_pos();
    std::vector<GLuint> get_surface_idx();

    void subdivision();

    std::vector<GLuint> find_surface_by_point(GLuint);
    std::vector<GLuint> find_adjacent_surfaces_for_edge(GLuint,GLuint,std::vector<GLuint>);
    std::vector<GLuint> arrange_surfaces(GLuint,std::vector<GLuint>);
    std::vector<GLuint> get_con_edge_by_surface(GLuint,GLuint);
    void gen_tree(std::string);
    void clear();
};


#endif //LAB03_TREE_H
