//
// @Author: Zhiquan Wang
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include "ZWEngine.h"

void bind_vertex_buffer_object(const std::vector<GLfloat> &data, GLenum data_type) {
    GLuint vbo_id = -1;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], data_type);
}

void bind_vertex_attribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                           const void *pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}

ZWEngine::ZWEngine() : delta_time(0), last_time(0) {
    this->window = nullptr;
    this->shader_program = nullptr;
    std::cout << "initialing Opengl engine ..." << std::endl;
}

// ZWEngine Member Functions
bool ZWEngine::init_engine(int w, int h) {
    bool window_init = this->init_window(w, h);
    bool glad_init = ZWEngine::init_glad();
    this->init_imgui();
    this->main_camera.set_aspect((GLfloat) w / (GLfloat) h);
    return window_init && glad_init;
}

bool ZWEngine::init_shader_program(const GLchar *vs_shader, const GLchar *fs_shader) {
    this->shader_program = new ShaderProgram();
    shader_program->attach_shader("vertex_shader", vs_shader);
    shader_program->attach_shader("fragment_shader", fs_shader);
    shader_program->link_program();
    this->set_render_info();

}

void ZWEngine::run() {
    this->main_loop();
    this->cleanup();
}

void ZWEngine::add_vao(const std::string &name, VertexArrayObject &vao) {
    this->vao_map.insert(std::pair<std::string, VertexArrayObject>(name, vao));
}

void ZWEngine::activate_vao(const std::string &name) {
    glBindVertexArray(this->vao_map.at(name).id());
}

void ZWEngine::disable_vao() {
    glBindVertexArray(0);
}

void ZWEngine::add_texture(Texture tex) {
    this->texture_list.push_back(tex);
}

void ZWEngine::activate_texture(GLint index) {
    if (index == -1) {//activate all textures
        std::vector<Texture>::iterator itor;
        for (itor = this->texture_list.begin(); itor != this->texture_list.end(); ++itor) {
            itor->activate();
        }
    } else {
        this->texture_list[index].activate();
    }
}

// private member functions
bool ZWEngine::init_window(int width, int height) {
    this->window_size.x = width;
    this->window_size.y = height;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // params: width, height, title, monitor, opengl related}
    this->window = glfwCreateWindow(width, height, "opengl", nullptr, nullptr);
    if (this->window == nullptr) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(this->window);

    return true;
}

bool ZWEngine::init_glad() {
    return gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0;
}

bool ZWEngine::init_imgui() {
    //Set ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings

    const char *glsl_version = "#version 460 core";
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    return true;
}

void ZWEngine::main_loop() {
    while (!glfwWindowShouldClose(this->window)) {
        // update delta time
        GLfloat c_time = glfwGetTime();
        this->delta_time = c_time - this->last_time;
        this->last_time = c_time;
        //
        process_input();
        this->render_ui();
        this->render_world();
        glfwPollEvents(); // check for events
        glfwSwapBuffers(this->window);
    }
}


void ZWEngine::cleanup() {
    // clean vao
    std::map<std::string, VertexArrayObject>::iterator itor_vao;
    for (itor_vao = this->vao_map.begin(); itor_vao != this->vao_map.end(); ++itor_vao) {
        itor_vao->second.release();
    }
    //  clean textures
    std::vector<Texture>::iterator itor_tex;
    for (itor_tex = this->texture_list.begin(); itor_tex != this->texture_list.end(); ++itor_tex) {
        itor_tex->release();
    }
    //clean up imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // optional: de-allocate all resources once they've outlived their purpose:
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

bool ZWEngine::attach_camera(Camera camera) {
    this->main_camera = camera;
}

Camera &ZWEngine::get_camera() {
    return this->main_camera;
}
VertexArrayObject ZWEngine::get_vao(const std::string &name) {
    return this->vao_map[name];
}
