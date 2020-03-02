#include <glad/glad.h>

#include "ZWEngine.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <set>
#include "imgui.h"
#include "Point.h"
#include "Tree.h"
#include <algorithm>
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const GLchar *vs_shader_path = "../glsl/vertex_shader.glsl";
const GLchar *fs_shader_path = "../glsl/fragment_shader.glsl";

int main() {

    auto *tmp_app = new ZWEngine();

    if (!tmp_app->init_engine(SCR_WIDTH, SCR_HEIGHT)) {
        std::cout << "engine failed to initialize" << std::endl;
    } else {
        std::cout << "engine initialized successfully" << std::endl;
    }
    tmp_app->init_shader_program(vs_shader_path, fs_shader_path);
    std::cout << "engine start running" << std::endl;
    tmp_app->run();

}