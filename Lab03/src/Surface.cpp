//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include <Surface.h>

#include "Surface.h"

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
std::vector<GLuint> Surface::get_edges_by_p(GLuint id){
    GLuint tmp_id;
    for(int i =0 ;i < 4;++ i){
        if(id == this->indices[i]){
            tmp_id = id;
        }
    }
    std::vector<GLuint> tmp_list;
    tmp_list.emplace_back((id+1)%4);
    tmp_list.emplace_back((id-1+4)%4);

}

bool Surface::contain_edge(std::vector<GLuint> edge,int &other0,int &other1){
    bool b0 = false;
    bool b1 = false;
    std::vector<GLuint> tmp;
    for(int i =0 ;i < 4;++ i){
        if(float(edge[0]) ==indices[i]){
            b0 = true;
        }else if(float(edge[1])==indices[i]){
            b1 = true;
        }else{
            tmp.emplace_back(indices[i]);
        }
    }
    other0 = tmp[0];
    other1 = tmp[1];
    return b0 && b1;
}
