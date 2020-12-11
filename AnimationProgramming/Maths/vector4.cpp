#include "Core/Maths/vector4.hpp"

Vector4::Vector4()
{
    this->x = this->y = this->z = this->w = 0;
}

Vector4::Vector4(float _x, float _y, float _z, float _w)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
    this->w = _w;
}

Vector4::Vector4(const Vector3 &vector3)
{
    x = vector3.x;
    y = vector3.y;
    z = vector3.z;
    w = 1.0f;
}

Vector4::Vector4(const Vector3 &vector3, float _w)
{
    x = vector3.x;
    y = vector3.y;
    z = vector3.z;
    this->w = _w;
}

void Vector4::homogenize()
{
    if (this->w >= 0.1f)
    {
        this->x /= this->w;
        this->y /= this->w;
        this->z /= this->w;
        this->w /= this->w;
    }
}

float Vector4::getMagnitude() const
{
    Vector4 vector;
    vector.homogenize();
    return sqrtf(powf(vector.x, 2) + powf(vector.y, 2) + powf(vector.z, 2));
}

Vector4::~Vector4()
{
}