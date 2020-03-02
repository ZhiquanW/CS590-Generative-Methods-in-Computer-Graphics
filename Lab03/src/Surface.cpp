//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include <Surface.h>

#include "Surface.h"
#include <iostream>
GLuint Surface::counter = 0;
Surface::Surface():id(counter++) {

}

Surface::Surface(glm::vec4 ids):id(counter++),indices(ids){

}

bool Surface::is_contain_point(GLuint pid) {
    for(int i =0 ;i < 4;++ i){
        if(pid == this->indices[i]){
            return true;
        }
    }
    return false;
}

void Surface::set_center_pt(glm::vec3 c){
    this->center_pt = c;
}
glm::vec2 Surface::get_edges_by_p(GLuint in_id){
    GLuint tmp_id = -1;
    for(int i =0 ;i < 4;++ i){
        if(in_id == this->indices[i]){
            tmp_id = i;
            break;
        }
    }
    return glm::vec2(this->indices[(tmp_id+1)%4],this->indices[(tmp_id+3)%4]);
}

bool Surface::contain_edge(std::vector<GLuint> edge){
    bool b0 = false;
    bool b1 = false;
    for(int i =0 ;i < 4;++ i){
        if(float(edge[0]) ==indices[i]){
            b0 = true;
        }else if(float(edge[1])==indices[i]){
            b1 = true;
        }
    }
    return b0 && b1;
}
