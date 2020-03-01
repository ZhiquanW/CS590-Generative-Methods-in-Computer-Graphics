//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include "Node.h"
GLuint Node::counter = 0;

Node::Node():id(counter++),pos(glm::vec3(0)){
    connections.resize(6);
    occupied_surfaces.resize(6);
}
Node::Node(glm::vec3 p):id(counter++),pos(p) {
    connections.resize(6);
    occupied_surfaces.resize(6);

}

void Node::add_connection(cnt_dir dir,GLuint id){
    if(dir <0 || dir > 5){
        std::cout << "ERROR in Node::add_connection" << std::endl;
        return;
    }
    connections[dir].push_back(id);
}
std::vector<GLuint> Node::get_connections(){
    std::vector<GLuint> tmp_list;
    std::vector<std::vector<GLuint> >::iterator itor_0;
    std::vector<GLuint>::iterator itor_1;
    for(itor_0 = connections.begin();itor_0 != connections.end();++ itor_0){
        for(itor_1 = itor_0->begin(); itor_1 != itor_0->end();++ itor_1){
            tmp_list.emplace_back(this->id);
            tmp_list.emplace_back(*itor_1);
            std::cout << this-> id << " --> " << *itor_1<< std::endl;
        }
    }
    return tmp_list;
}

bool Node::is_endpoint(){
    GLuint cnt_num = 0;
    std::vector<std::vector<GLuint> >::iterator itor;
    for(itor = connections.begin(); itor != connections.end(); ++ itor){
       cnt_num += itor->size();
    }
    return cnt_num == 0;
}

std::vector<GLuint> Node::unoccupied_dir() {
    std::vector<GLuint> occupied;
    for(int i =0 ;i < 6;++ i){
        if(occupied_surfaces[i]==0){
            occupied.emplace_back(i);
        }
    }
    return occupied;
}

void Node::add_occupied_surfaces(GLuint i) {
    this->occupied_surfaces[i] = 1;
}

void Node::add_vertex(GLuint n_id){
    this->vertices.emplace_back(n_id);
}

std::vector<GLuint > Node::get_vertices_by_dir(GLuint dir) {
    std::vector<glm::vec4> surface_dir_idx = {
            glm::vec4(2,3,7,6),
            glm::vec4(0,1,5,4),
            glm::vec4(0,1,3,2),
            glm::vec4(4,5,7,6),
            glm::vec4(1,3,7,5),
            glm::vec4(0,2,6,4),

    };
    std::vector<GLuint> tmp_list;
    tmp_list.emplace_back(this->vertices[surface_dir_idx[dir].x]);
//    std::cout << this->vertices[surface_dir_idx[dir].x]->id << std::endl;
    tmp_list.emplace_back(this->vertices[surface_dir_idx[dir].y]);
    tmp_list.emplace_back(this->vertices[surface_dir_idx[dir].z]);
    tmp_list.emplace_back(this->vertices[surface_dir_idx[dir].w]);
    return tmp_list;
}
