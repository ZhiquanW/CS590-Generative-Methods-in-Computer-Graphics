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
    GLfloat offset = 1.f;
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
            tmp_id.emplace_back(tmp_point.id);
            itor_n->add_vertex(tmp_point.id);
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
    std::vector<Surface> new_surfaces;
    std::vector<Point> new_points(this->points);
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
//    std::vector<Point>::iterator itor;
    GLuint old_size = this->points.size();
//    Surface::counter = 0;
//    Point::counter = 0;
    for(int p_id = 0;p_id <old_size;++ p_id){
        glm::vec3 sum_surfaces_points(0);
        glm::vec3 sum_edge_points(0);
//        std::cout << p_id << std::endl;
        std::vector<GLuint> tmp_new_edge_points;
        std::vector<GLuint> new_face_points;
        //find adjacent surfaces
        std::vector<GLuint> connected_surfaces_id = this->find_surface_by_point(this->points[p_id].id);
//        std::cout << this->points[p_id].id  << std::endl;
//        std::cout << connected_surfaces_id.size() << std::endl;
        //arrange surfaces
        std::vector<GLuint> arranged_surfaces_id = this->arrange_surfaces(this->points[p_id].id,connected_surfaces_id);
        //generate new face points
        std::vector<GLuint>::iterator itor_as;
        for(itor_as = arranged_surfaces_id.begin();itor_as != arranged_surfaces_id.end();++ itor_as){
            Point tmp_p = Point(this->surfaces[*itor_as].center_pt);
            new_points.emplace_back(tmp_p);
            new_face_points.emplace_back(tmp_p.id);
            sum_surfaces_points += tmp_p.pos;
        }
        // generate new edge points
//        std::cout << arranged_surfaces_id.size() << std::endl;
        for(int i =0;i < arranged_surfaces_id.size();++ i){
            GLuint sur_id_0 = arranged_surfaces_id[i];
            GLuint sur_id_1 = arranged_surfaces_id[(i+1)%arranged_surfaces_id.size()];
            glm::vec3 tmp_pos = this->surfaces[sur_id_0].center_pt +
                                this->surfaces[sur_id_1].center_pt;
            std::vector<GLuint> con_edge_p = this->get_con_edge_by_surface(sur_id_0,sur_id_1);
            tmp_pos += this->points[con_edge_p[0]].pos + this->points[con_edge_p[1]].pos;
            tmp_pos /=4;
            sum_edge_points += tmp_pos;
            Point tmp_p(tmp_pos, false);
            auto tmp_p_ptr = std::find(this->points.begin(),this->points.end(),tmp_p);
            if(tmp_p_ptr != this->points.end()){
                tmp_new_edge_points.emplace_back(tmp_p_ptr->id);
            }else{
                tmp_p.id = Point::counter++;
                new_points.emplace_back(tmp_p);
                tmp_new_edge_points.emplace_back(tmp_p.id);
            }
        }
        // generate new surfaces
        for(int i =0 ;i < new_face_points.size();++ i){
            GLuint id_0 = new_face_points[i];
            GLuint id_1 = tmp_new_edge_points[i];
            GLuint id_2 = this->points[p_id].id;
            GLuint id_3 = tmp_new_edge_points[(i-1+tmp_new_edge_points.size())%tmp_new_edge_points.size()];
            new_surfaces.emplace_back(Surface(glm::vec4(id_0,id_1,id_2,id_3)));
//            std::cout  << "s " << Surface::counter << std::endl;
        }
        // update old points position
        GLfloat n = connected_surfaces_id.size();
        sum_surfaces_points/=n;
        sum_edge_points/=n;
        new_points[p_id].pos = this->points[p_id].pos*(n-3.0f)/n+ sum_edge_points*2.0f/n + sum_surfaces_points/n;
    }

    this->points = new_points;
    this->surfaces = new_surfaces;

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

std::vector<GLuint> Tree::arrange_surfaces(GLuint c,std::vector<GLuint> surfaces_idx) {
   std::vector<GLuint> out_list;
    GLuint first_sur = *(surfaces_idx.end() - 1);
    out_list.emplace_back(first_sur);
    //get start edge;
    std::vector<GLuint> first_edge;
    first_edge.emplace_back(c);
    first_edge.emplace_back(this->surfaces[first_sur].get_edges_by_p(c).x);

    std::vector<GLuint>::iterator itor;
    GLuint s_size = surfaces_idx.size()-1;
    for(int i= 0;i < s_size;++ i){
            for(itor = surfaces_idx.begin(); itor != surfaces_idx.end(); ++ itor){
                if(this->surfaces[*itor].contain_edge(first_edge)){
                    first_edge.clear();
                    out_list.emplace_back(*itor);
                    glm::vec2 ps = this->surfaces[*itor].get_edges_by_p(c);
                    GLuint other_p = ps.x==first_edge[1] ? ps.y : ps.x;
                    first_edge.push_back(c);
                    first_edge.push_back(other_p);
                    surfaces_idx.erase(itor);
                    break;
                }
            }
    }
    return out_list;
}

std::vector<GLuint> Tree::get_con_edge_by_surface(GLuint s0, GLuint s1) {
    std::vector<GLuint> con_edge_ps;
    for(int i =0;i < 4;++ i){
        for(int j =0 ;j < 4;++ j){
            if(this->surfaces[s0].indices[i] == this->surfaces[s1].indices[j]){
                con_edge_ps.emplace_back(this->surfaces[s0].indices[i]);
            }
        }
    }
    return con_edge_ps;

}
