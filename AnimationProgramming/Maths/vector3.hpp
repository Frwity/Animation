#ifndef __VECTOR3_HPP__
#define __VECTOR3_HPP__

#include <math.h>

class Vector3
{

public:
    float x, y, z;

    Vector3();
    Vector3(float x, float y, float z);
    ~Vector3();
    float getMagnitude() const;
    float dotProduct(Vector3 v) const;
    float triangleArea(Vector3 v) const;
    unsigned int angleBtwVect(Vector3 vector) const;
    Vector3 cross(Vector3 v) const;
    bool isVecColinear(Vector3 v) const;
    Vector3 vectorProduct(Vector3 v) const;
    float CrossProduct2(Vector3 v); 
    void normalize();
    Vector3 normalized() const;

    
    Vector3 operator+(Vector3 vector) const
    {
        Vector3 to_return;
        to_return.x = vector.x + this->x;
        to_return.y = vector.y + this->y;
        to_return.z = vector.z + this->z;
        return to_return;
    }
    Vector3 operator-(Vector3 vector) const
    {
        Vector3 to_return;
        to_return.x = this->x - vector.x;
        to_return.y = this->y - vector.y;
        to_return.z = this->z - vector.z;
        return to_return;
    }
    Vector3 operator*(float number) const
    {
        Vector3 to_return;

        to_return.x = this->x * number;
        to_return.y = this->y * number;
        to_return.z = this->z * number;
        return to_return;
    }
    Vector3 operator*=(float number)
    {
        this->x *= number;
        this->y *= number;
        this->z *= number;
        return *this;
    }
    Vector3 operator/(float number) const
    {
        Vector3 to_return;
        to_return.x = this->x / number;
        to_return.y = this->y / number;
        to_return.z = this->z / number;
        return to_return;
    }
};

#endif