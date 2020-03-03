//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//
#include "ZWEngine.h"

//#define STB_IMAGE_IMPLEMENTATION
//
//#include "stb_image.h"
static ZWEngine *self;

// Opengl functions
void ZWEngine::set_render_info() {
    // Set Render
    self = this;
    /* Customize */
    /*=========================== Enable OpenGL Features ===========================*/
    glEnable(GL_DEPTH_TEST);
    /*=========================== Setup Window Interactions ===========================*/
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    /*=========================== Setup Shader Program ===========================*/
    shader_program->use_shader_program();
    /*=========================== Setup Camera ===========================*/
    Camera main_cam;
    main_camera.set_pos(glm::vec3(0, 2.5, 4));
    main_camera.set_pitch_yaw(glm::vec2(-30,-90));
    this->attach_camera(main_camera);
    /*=========================== Add Objects ===========================*/
    // init tree nodes
    my_tree.height = 0.0f;
    my_tree.offset = 0.2f;
    my_tree.clear();
    this->my_tree.add_node(0, up, Node(glm::vec3(0, 1 - this->my_tree.height, 0)));
    this->my_tree.add_node(1, right, Node(glm::vec3(0.4, 1.2 - this->my_tree.height, 0.1)));
    this->my_tree.add_node(1, up, Node(glm::vec3(-0.25, 1.9 - this->my_tree.height, -0.15)));
    this->my_tree.add_node(1, left, Node(glm::vec3(-0.5, 1.3 - this->my_tree.height, 0.2)));
    this->my_tree.add_node(4, up, Node(glm::vec3(-0.5, 1.8 - this->my_tree.height, 0.5)));
    std::vector<glm::vec3> nodes_pos = this->my_tree.generate_points();
    std::vector<GLuint> connections = this->my_tree.generate_connections();
    my_tree.generate_surfaces();
    std::vector<glm::vec3> position_list = this->my_tree.get_point_pos();
    std::vector<GLuint> idx = this->my_tree.get_surface_idx();
    VertexArrayObject surfaces_vao(true);
    VertexBufferObject surfaces_vbo(position_list, GL_STATIC_DRAW);
    ElementBufferObject surfaces_ebo(idx, GL_STATIC_DRAW);
    //pos
    bind_vertex_attribute(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) nullptr);
    surfaces_vao.attach_vbo(surfaces_vbo.id());
    surfaces_vao.attach_ebo(surfaces_ebo.id());
    surfaces_vao.set_elements_num(idx.size());
    surfaces_vao.activate();
    this->add_vao("surfaces", surfaces_vao);
    ZWEngine::disable_vao();

//    // init tree surfaces
//    my_tree.generate_surfaces();
//    my_tree.subdivision();
//    my_tree.subdivision();
//    my_tree.subdivision();
//    my_tree.subdivision();
//    my_tree.subdivision();
}

void ZWEngine::render_ui() {
    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Create a window called "Hello, world!" and append into it.
    ImGui::Begin("Hello, world!");
    if (this->uniform_failed_id != -1) {
        std::string tmp = "uniform variable ";
        tmp += std::to_string(uniform_failed_id);
        tmp += " declare failed";
        ImGui::Text("%s", tmp.c_str());
    }

    ImGui::SliderFloat("obj angle: ", &obj_angle, -180.0f, 180.0f);
    ImGui::SliderFloat2("camera angle", &this->main_camera.get_pitch_yaw()[0], -180, 180);
    ImGui::SliderFloat("width: ",&this->my_tree.offset,0.1f,0.4f);
    ImGui::SliderFloat("angle: ",&this->my_tree.height,0.0f,0.5f);
    if(ImGui::Button("Generate")){
        this->regenerate();
    }if(ImGui::Button("Subdivision")){
//        this->my_tree.clear();
        this->my_tree.subdivision();
        std::vector<glm::vec3> position_list = this->my_tree.get_point_pos();
        std::vector<GLuint> idx = this->my_tree.get_surface_idx();
        VertexArrayObject surfaces_vao(true);
        VertexBufferObject surfaces_vbo(position_list, GL_STATIC_DRAW);
        ElementBufferObject surfaces_ebo(idx, GL_STATIC_DRAW);
        //pos
        bind_vertex_attribute(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) nullptr);
        surfaces_vao.attach_vbo(surfaces_vbo.id());
        surfaces_vao.attach_ebo(surfaces_ebo.id());
        surfaces_vao.set_elements_num(idx.size());
        surfaces_vao.activate();
        this->vao_map.clear();
        this->add_vao("surfaces", surfaces_vao);
        ZWEngine::disable_vao();
    }
    ImGui::End();
    ImGui::Render();
}

void ZWEngine::render_world() {
    shader_program->use_shader_program();

    // clear buffers
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::rotate(glm::radians(this->obj_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    if (!shader_program->set_uniform_mat4fv(2, model)) {
        this->uniform_failed_id = 2;
    }
    glm::mat4 view = this->main_camera.get_view_mat();
    if (!shader_program->set_uniform_mat4fv(3, view)) {
        this->uniform_failed_id = 3;
    }
    glm::mat4 proj = this->main_camera.get_projection_mat();
    if (!shader_program->set_uniform_mat4fv(4, proj)) {
        this->uniform_failed_id = 4;
    }
    this->activate_vao("surfaces");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES,this->vao_map["surfaces"].get_elements_num() , GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    ZWEngine::disable_vao();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void ZWEngine::process_input() {
    // check 'ESC' is pressed
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, true);
    }
}


void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
    self->get_camera().set_aspect((GLfloat) w / (GLfloat) h);
}

bool first_in = true;
glm::vec2 pre_mouse_pos;
// callback function


void ZWEngine::keycode_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        self->main_camera.process_keyboard(FORWARD, self->delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        self->main_camera.process_keyboard(BACKWARD, self->delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        self->main_camera.process_keyboard(LEFT, self->delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        self->main_camera.process_keyboard(RIGHT, self->delta_time);
}

void ZWEngine::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (first_in) {
        pre_mouse_pos = glm::vec2(xpos, ypos);
        first_in = false;
    }
    glm::vec2 offset(xpos - pre_mouse_pos.x, pre_mouse_pos.y - ypos);
    pre_mouse_pos = glm::vec2(xpos, ypos);
    self->main_camera.process_mouse_movement(offset);
}

void ZWEngine::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    self->main_camera.process_mouse_scroll(yoffset);
}






