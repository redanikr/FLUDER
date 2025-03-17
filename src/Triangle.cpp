#include "Triangle.h"


fldr::Triangle::Triangle(float x, float y, float z) : m_x(x), m_y(y), m_z(z){}
float fldr::Triangle::x()const{return m_x;}
float fldr::Triangle::y()const{return m_y;}
float fldr::Triangle::z()const{return m_z;}