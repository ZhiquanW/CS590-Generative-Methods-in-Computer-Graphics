//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef LAB03_TREE_H
#define LAB03_TREE_H
#include "Node.h"



class Tree {
public:
    std::vector<Node> nodes;
    Tree(){
        nodes.emplace_back(glm::vec3(0));
    }

    bool add_node(GLuint id,cnt_dir dir,Node n){
        //dir 0 up 1 down 2 left 3 right
        if(id < nodes.size()){
            nodes.push_back(n);
            nodes[id].add_connection(dir, &*(nodes.end() - 1));
        }
    }
    std::vector<glm::vec3> generate_points(){
        std::vector<glm::vec3> tmp_list;
        std::vector<Node>::iterator itor;
        for(itor = nodes.begin(); itor != nodes.end();++ itor){
            tmp_list.emplace_back(itor->pos);
            std::cout << itor->id << " : "<< itor->pos.x << " , " << itor-> pos.y << " , " << itor->pos.z << std::endl;
        }
        return tmp_list;
    }
    std::vector<GLuint> generate_connections(){
        std::vector<GLuint> tmp_list;
        std::vector<Node>::iterator itor;
        for(itor = nodes.begin();itor != nodes.end()-1;++ itor){
            std::vector<GLuint> new_connections = itor->get_connections();
            tmp_list.insert(tmp_list.end(),new_connections.begin(),new_connections.end());
        }
        return tmp_list;
    }

};


#endif //LAB03_TREE_H
