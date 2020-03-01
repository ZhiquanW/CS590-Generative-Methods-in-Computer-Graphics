//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include <Point.h>

#include "Point.h"
GLuint Point::counter;

Point::Point():id(counter++) {
}

Point::Point(glm::vec3 p):id(counter++),pos(p){
}
bool Point::operator <(const Point p) const
{
    if(this->pos.x == p.pos.x){
        if(this->pos.y == p.pos.y){
            return this->pos.z < p.pos.z;
        }else{
            return this->pos.y < p.pos.y;
        }
    } else{
        return this->pos.x < p.pos.x;
    }
}