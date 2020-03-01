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
