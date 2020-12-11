#ifndef VECTOR4
#define VECTOR4

#include "vector3.hpp"

class Vector4
{
public:
    float x, y, z, w;

    Vector4();
    Vector4(float _x, float _y, float _z, float _w);
    Vector4(const Vector3 &vector3);
    Vector4(const Vector3 &vector3, float _w);
    ~Vector4();

    void homogenize();
    float getMagnitude() const;
};

#endif