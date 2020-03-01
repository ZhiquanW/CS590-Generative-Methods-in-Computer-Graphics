//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//


//#include <Tree.h>

#include "Tree.h"
Tree::Tree(){
    nodes.emplace_back(glm::vec3(0));
}

bool Tree::add_node(GLuint id,cnt_dir dir,Node &n){
    //dir 0 up 1 down 2 left 3 right 4 front 5 back
    if(id < nodes.size()){
        n.add_occupied_surfaces(dir + pow(-1,dir%2));
        nodes.push_back(n);
        nodes[id].add_connection(dir, &n);
        nodes[id].add_occupied_surfaces(dir);
    }
}
std::vector<glm::vec3> Tree::generate_points(){
    std::vector<glm::vec3> tmp_list;
    std::vector<Node>::iterator itor;
    for(itor = nodes.begin(); itor != nodes.end();++ itor){
        tmp_list.emplace_back(itor->pos);
    }
    return tmp_list;
}
std::vector<GLuint> Tree::generate_connections(){
    std::vector<GLuint> tmp_list;
    std::vector<Node>::iterator itor;
    for(itor = nodes.begin();itor != nodes.end()-1;++ itor){
        std::vector<GLuint> new_connections = itor->get_connections();
        tmp_list.insert(tmp_list.end(),new_connections.begin(),new_connections.end());
    }
    return tmp_list;
}
void Tree::generate_surfaces(){
    //Generate surfaces for base point
    std::vector<glm::vec3> dirs = {glm::vec3 (-1,-1,-1),
                                   glm::vec3 (-1,-1,1),
                                   glm::vec3 (-1,1,-1),
                                   glm::vec3 (-1,1,1),
                                   glm::vec3 (1,-1,-1),
                                   glm::vec3 (1,-1,1),
                                   glm::vec3 (1,1,-1),
                                   glm::vec3 (1,1,1),};
    std::vector<glm::vec4> surface_dir_idx = {
            glm::vec4(2,3,7,6),
            glm::vec4(0,1,5,4),
            glm::vec4(0,1,3,2),
            glm::vec4(4,5,7,6),
            glm::vec4(1,3,7,5),
            glm::vec4(0,2,6,4),

    };
    GLfloat offset = 0.1f;
    std::vector<Node>::iterator itor_n;
    std::vector<glm::vec3>::iterator itor_d;
    for(itor_n = nodes.begin();itor_n != nodes.end();++ itor_n){
        //generate 8 points
        std::vector<GLuint> tmp_id;
        std::cout << itor_n->id << std::endl;
        for(itor_d = dirs.begin(); itor_d != dirs.end(); ++ itor_d){
            Point tmp_point(itor_n->pos);
            tmp_point.pos += *itor_d * offset;
            this->points.emplace_back(tmp_point);
            tmp_id.emplace_back(tmp_point.id);
            itor_n->add_vertex(&tmp_point);
        }
        // generate empty surfaces
        std::vector<GLuint> unoccpied = itor_n->unoccupied_dir();
        std::vector<GLuint>::iterator itor_u;
        for(itor_u = unoccpied.begin();itor_u != unoccpied.end();++ itor_u){
            GLuint os_id = *itor_u;
            std::cout << tmp_id[surface_dir_idx[os_id].x] << " , "<< tmp_id[surface_dir_idx[os_id].y] <<" , " << tmp_id[surface_dir_idx[os_id].z]<<" , " << tmp_id[surface_dir_idx[os_id].w] << std::endl;
            this->surfaces.emplace_back(Surface(glm::vec4(tmp_id[surface_dir_idx[os_id].x],tmp_id[surface_dir_idx[os_id].y],tmp_id[surface_dir_idx[os_id].z],tmp_id[surface_dir_idx[os_id].w])));
        }
    }
    std::vector<std::vector<Node*> >::iterator itor_c;
    for(itor_n = nodes.begin();itor_n != nodes.end();++ itor_n) {
        GLuint dir_counter = 0;
        for(itor_c = itor_n->connections.begin(); itor_c != itor_n->connections.end();++ itor_c){
            if(!itor_c->empty()){
                std::vector<Point*> self_list = itor_n->get_vertices_by_dir(dir_counter);
                std::vector<Point*> other_list =(*itor_c->begin())->get_vertices_by_dir(dir_counter + pow(-1,dir_counter%2 ));
             //   this->surfaces.emplace_back(Surface(glm::vec4(self_list[0]->id,self_list[1]->id,other_list[0]->id,other_list[1]->id)));
              //  this->surfaces.emplace_back(Surface(glm::vec4(self_list[0]->id,self_list[3]->id,other_list[0]->id,other_list[3]->id)));
               // this->surfaces.emplace_back(Surface(glm::vec4(self_list[2]->id,self_list[1]->id,other_list[2]->id,other_list[1]->id)));
                //this->surfaces.emplace_back(Surface(glm::vec4(self_list[2]->id,self_list[3]->id,other_list[2]->id,other_list[3]->id)));

            }
            dir_counter++;
        }
    }

}

std::vector<glm::vec3> Tree::get_point_pos() {
    std::vector<glm::vec3> point_pos;
    std::vector<Point>::iterator itor;
    for(itor = this->points.begin(); itor != this->points.end();++ itor){
        point_pos.emplace_back(itor->pos);
    }
    return point_pos;
}

std::vector<GLuint> Tree::get_surface_idx() {
    std::vector<GLuint> surface_idx;
    std::vector<Surface>::iterator itor;
    for(itor = this->surfaces.begin(); itor != this->surfaces.end();++ itor){
        surface_idx.emplace_back(itor->indices[0]);
        surface_idx.emplace_back(itor->indices[1]);
        surface_idx.emplace_back(itor->indices[2]);
        surface_idx.emplace_back(itor->indices[0]);
        surface_idx.emplace_back(itor->indices[2]);
        surface_idx.emplace_back(itor->indices[3]);
    }
    return surface_idx;
}
