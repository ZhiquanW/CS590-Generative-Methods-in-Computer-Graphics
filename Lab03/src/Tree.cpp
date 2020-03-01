//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//


//#include <Tree.h>

#include <set>
#include "Tree.h"

Tree::Tree(){
    nodes.emplace_back(Node(glm::vec3(0,0,0)));
}

bool Tree::add_node(GLuint id,cnt_dir dir,Node n){
    //dir 0 up 1 down 2 left 3 right 4 front 5 back
    if(id < nodes.size()){
        n.add_occupied_surfaces(dir + pow(-1,dir%2));
        nodes.push_back(n);
        nodes[id].add_connection(dir, n.id);
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
    for(itor_n = nodes.begin();itor_n != nodes.end();++ itor_n){
        //generate 8 points
        std::vector<GLuint> tmp_id;
//        std::cout << itor_n->id << std::endl;
        std::vector<glm::vec3>::iterator itor_d;
        for(itor_d = dirs.begin(); itor_d != dirs.end(); ++ itor_d){
            Point tmp_point(itor_n->pos);
            tmp_point.pos += *itor_d * offset;
            this->points.emplace_back(tmp_point);
            tmp_id.emplace_back(points.size());
            itor_n->add_vertex(points.size());
        }
        // generate empty surfaces
        std::vector<GLuint> unoccpied = itor_n->unoccupied_dir();
        std::vector<GLuint>::iterator itor_u;
        for(itor_u = unoccpied.begin();itor_u != unoccpied.end();++ itor_u){
            GLuint os_id = *itor_u;
//            std::cout << tmp_id[surface_dir_idx[os_id].x] << " , "<< tmp_id[surface_dir_idx[os_id].y] <<" , " << tmp_id[surface_dir_idx[os_id].z]<<" , " << tmp_id[surface_dir_idx[os_id].w] << std::endl;
            this->surfaces.emplace_back(Surface(glm::vec4(tmp_id[surface_dir_idx[os_id].x],tmp_id[surface_dir_idx[os_id].y],tmp_id[surface_dir_idx[os_id].z],tmp_id[surface_dir_idx[os_id].w])));
        }
    }
    std::vector<std::vector<GLuint> >::iterator itor_c;
    for(itor_n = nodes.begin();itor_n != nodes.end();++ itor_n) {
        GLuint dir_counter = 0;
        for(itor_c = itor_n->connections.begin(); itor_c != itor_n->connections.end();++ itor_c){
            if(!itor_c->empty()){
                std::vector<GLuint> self_list = itor_n->get_vertices_by_dir(dir_counter);
                std::vector<GLuint> other_list =this->nodes[*(itor_c->begin())].get_vertices_by_dir(dir_counter + pow(-1,dir_counter%2 ));
                this->surfaces.emplace_back(Surface(glm::vec4(self_list[0],self_list[1],other_list[1],other_list[0])));
                this->surfaces.emplace_back(Surface(glm::vec4(self_list[0],self_list[3],other_list[3],other_list[0])));
                this->surfaces.emplace_back(Surface(glm::vec4(self_list[2],self_list[1],other_list[1],other_list[2])));
                this->surfaces.emplace_back(Surface(glm::vec4(self_list[2],self_list[3],other_list[3],other_list[2])));

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

void Tree::subdivision(){
    // calculcate new surfaces points, do not push
    std::vector<Surface>::iterator itor_f;
    for(itor_f = this->surfaces.begin();itor_f != this->surfaces.end();++ itor_f){
        glm::vec3 tmp_c(0.0f);
        for(int i =0 ;i < 4;++ i){
            tmp_c += this->points[itor_f->indices[i]].pos;
        }
        itor_f->center_pt = tmp_c/4.0f;
    }
    //update vertex position
    std::vector<Point>::iterator itor;
    GLuint p_id = 0;
    for(itor = this->points.begin();itor != this->points.end();++ itor){
        std::set<Point> tmp_new_edge_points;
        std::vector<GLuint> new_face_points;
        //find adjacent surfaces
        std::vector<GLuint> connected_surfaces_id = this->find_surface_by_point(p_id++);
        //find adjacent edge
        std::vector<GLuint> edges;
        std::vector<GLuint>::iterator itor_cs;
        for(itor_cs = connected_surfaces_id.begin();itor_cs != connected_surfaces_id.end();++ itor_cs){
            std::vector<GLuint> tmp_edges = this->surfaces[*itor_cs].get_edges_by_p(itor->id);
            edges.emplace_back(tmp_edges[0]);
            edges.emplace_back(tmp_edges[1]);
        }

    }
};

std::vector<GLuint>Tree::find_surface_by_point(GLuint id) {
    std::vector<GLuint> tmp_list;
    std::vector<Surface>::iterator itor;
    for(itor = this->surfaces.begin();itor!= this->surfaces.end();++ itor){
        if(itor->is_contain_point(id)){
            tmp_list.emplace_back(itor->id);
        }
    }
    return tmp_list;
}


std::vector<GLuint> Tree::find_adjacent_surfaces_for_edge(GLuint e0,GLuint e1,std::vector<GLuint> surfaces_id){
    std::vector<GLuint>::iterator itor;
    for(itor = surfaces_id.begin(); itor != surfaces_id.end();++ itor){

    }
}

std::vector<GLuint> Tree::arrange_surfaces(GLuint c,std::vector<GLuint> surfaces) {
    std::vector<GLuint> out_list;
    GLuint first_sur = *(surfaces.end()-1);
    out_list.emplace_back(first_sur);
    //get start edge;
    std::vector<GLuint> first_edge;
    first_edge.emplace_back(c);
    first_edge.emplace_back(this->surfaces[first_sur].get_edges_by_p(c)[0]);
    std::vector<GLuint> sec_edge;
    first_edge.emplace_back(c);
    first_edge.emplace_back(this->surfaces[first_sur].get_edges_by_p(c)[1]);
    //
    std::vector<GLuint>::iterator itor;
    for(int i= 0;i < first_edge.size();++ i){
            for(itor = surfaces.begin();itor != surfaces.end();++ itor){
                int p0 = 0;
                int p1 = 1;
                if(this->surfaces[*itor].contain_edge(first_edge,p0,p1)){
                    out_list.emplace_back(*itor);
                    surfaces.erase(itor);
                    first_edge.clear();
                    first_edge.push_back(c);
                    first_edge.push_back(p1);
                }
            }
    }
}