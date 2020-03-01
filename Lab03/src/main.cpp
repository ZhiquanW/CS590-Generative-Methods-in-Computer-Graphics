#include <glad/glad.h>

#include "ZWEngine.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <set>
#include "imgui.h"
#include "Point.h"
#include "Tree.h"
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const GLchar *vs_shader_path = "../glsl/vertex_shader.glsl";
const GLchar *fs_shader_path = "../glsl/fragment_shader.glsl";

int main() {
    std::set<Point> tmp_p;
    Point p0(glm::vec3(0,1,1));
    Point p1(glm::vec3(-1,1,1));
    Point p2(glm::vec3(-1,1,1));
    tmp_p.insert(p0);
    tmp_p.insert(p1);
    tmp_p.insert(p2);
    Point a(glm::vec3(-1,1,1));

   std::cout <<tmp_p.count(a) << std::endl;
//    auto *tmp_app = new ZWEngine();
//
//    if (!tmp_app->init_engine(SCR_WIDTH, SCR_HEIGHT)) {
//        std::cout << "engine failed to initialize" << std::endl;
//    } else {
//        std::cout << "engine initialized successfully" << std::endl;
//    }
//    tmp_app->init_shader_program(vs_shader_path, fs_shader_path);
//    std::cout << "engine start running" << std::endl;
//    tmp_app->run();
//    Tree my_tree;
//    my_tree.add_node(0,up,Node(glm::vec3(0,1,0)));
//    my_tree.add_node(1,right,Node(glm::vec3(0.25,1.4,0.4)));
}