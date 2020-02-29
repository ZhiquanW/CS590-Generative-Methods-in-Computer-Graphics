//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef HELLO_WORLD_A_H
#define HELLO_WORLD_A_H

#include <glad/glad.h>
#include <vector>
#include <set>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram {
private:
    int success_flag{};
    static const short loginfo_len = 512;
    char log_info[loginfo_len]{};
    unsigned int shader_program_id;
    std::vector<unsigned int> shader_id_list;
    std::set<GLuint> uniform_id_set;
public:
    ShaderProgram();

    void use_shader_program();

    void attach_shader(const char *, const GLchar *);

    void link_program();

    GLuint id();

    bool check_uniform_loc(GLuint);

    bool set_uniform_mat4fv(GLuint, glm::mat4 &, bool= false);
};


#endif //HELLO_WORLD_A_H
