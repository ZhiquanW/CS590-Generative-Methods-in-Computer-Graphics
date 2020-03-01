//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include <Point.h>

#include "Point.h"
GLuint Point::counter;

Point::Point():id(Point::counter++) {
}

Point::Point(glm::vec3 p):id(Point::counter++),pos(p){
}