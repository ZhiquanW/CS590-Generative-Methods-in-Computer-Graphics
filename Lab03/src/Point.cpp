//
// @Author: Zhiquan Wang 
// @Date: 2/29/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include <Point.h>

#include "Point.h"
GLuint Point::counter;

Point::Point():id(-1) {
}

Point::Point(glm::vec3 p,bool increase):pos(p){
    id = increase?counter++ : -1;
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
bool Point::operator ==(const Point p) const {
//    return true;
    return abs(this->pos.x - p.pos.x) < 0.001f && abs(this->pos.y - p.pos.y)<0.001f && abs(this->pos.z - p.pos.z)<0.001f;
}