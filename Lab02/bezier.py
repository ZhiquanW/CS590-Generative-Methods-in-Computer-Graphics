import scipy 
from scipy import special
import sys
import numpy as np

def cal_loss(*,sample_points,line_points):
    total_dis = 0
    counter = 0
    for p in line_points:
        tmp_index = round(counter/len(line_points) * (len(sample_points)-1))
        total_dis += np.linalg.norm(p - sample_points[tmp_index])
        counter += 1
    return total_dis

class bezier(object):
    def __init__(self,*,control_points):
        self.__control_points = np.array(control_points)
        self.__degree = len(control_points)-1
    
    def get_point(self,*,t):
        points = np.zeros((1,2))
        for i in range(self.__degree+1):
            points +=  pow(1-t,self.__degree-i) * pow(t,i) * scipy.special.comb(self.__degree,i) * self.__control_points[i]
        return points

    def get_points(self,*,num):
        samples_on_line = np.array([self.get_point(t = tt) for tt in np.linspace(0,1,num)]).reshape(-1,2)
        return samples_on_line
   
    def update_point(self,*,id,value):
        self.__control_points[id] = value
    
    def get_control_points(self):
        return self.__control_points