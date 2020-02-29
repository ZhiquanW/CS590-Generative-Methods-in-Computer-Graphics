//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include "Node.h"
GLuint Node::counter = 0;

Node::Node():id(counter++),pos(glm::vec3(0)){
    connections.resize(6);
}
Node::Node(glm::vec3 p):id(counter++),pos(p) {
    connections.resize(6);
}

void Node::add_connection(cnt_dir dir, Node *n){
    if(dir <0 || dir > 5){
        std::cout << "ERROR in Node::add_connection" << std::endl;
        return;
    }
    connections[dir].push_back(n);
}
std::vector<GLuint> Node::get_connections(){
    std::vector<GLuint> tmp_list;
    std::vector<std::vector<Node *> >::iterator itor_0;
    std::vector<Node *>::iterator itor_1;
    for(itor_0 = connections.begin();itor_0 != connections.end();++ itor_0){
        for(itor_1 = itor_0->begin(); itor_1 != itor_0->end();++ itor_1){
            tmp_list.emplace_back(this->id);
            tmp_list.emplace_back((*itor_1)->id);
            std::cout << this-> id << " --> " << (*itor_1)->id << std::endl;
        }
    }
    return tmp_list;
}

