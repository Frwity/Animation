#include "Core/Maths/vector3.hpp"

Vector3::Vector3(/* args */)
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3::Vector3(float _x, float _y, float _z)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
}

float Vector3::getMagnitude() const
{
    return sqrtf(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2));
}

Vector3 Vector3::vectorProduct(Vector3 v) const
{
    Vector3 to_return;
    to_return.x = this->y * v.z - this->z * v.y;
    to_return.y = this->z * v.x - this->x * v.z;
    to_return.z = this->x * v.y - this->y * v.x;
    return to_return;
}

float Vector3::CrossProduct2(Vector3 v)
{
    return (this->x * v.y - this->y * v.x);
}

Vector3 Vector3::normalized() const
{
    Vector3 to_return;
    float magnitude = this->getMagnitude();
    to_return.x = this->x / magnitude;
    to_return.y = this->y / magnitude;
    to_return.z = this->z / magnitude;
    return to_return;
}

void Vector3::normalize()
{
    float magnitude = this->getMagnitude();
    this->x /= magnitude;
    this->y /= magnitude;
    this->z /= magnitude;
}

Vector3 Vector3::cross(Vector3 v) const
{
    Vector3 to_return;
    to_return.x = this->y * v.z - this->z * v.y;
    to_return.y = this->z * v.x - this->x * v.z;
    to_return.z = this->x * v.y - this->y * v.x;
    return to_return;
}


float Vector3::dotProduct(Vector3 v) const
{
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

unsigned int Vector3::angleBtwVect(Vector3 vector) const
{

    float fst_vector_length = this->getMagnitude();
    float scd_vector_length = vector.getMagnitude();
    unsigned int angle = acos(dotProduct(vector) / fst_vector_length * scd_vector_length);
    return angle;
}

bool Vector3::isVecColinear(Vector3 v) const
{
    if ((this->cross(v).getMagnitude() <= 0.01) && (this->cross(v).getMagnitude() >= -0.01))
        return true;
    return false;
}

float Vector3::triangleArea(Vector3 v) const
{
    return this->cross(v).getMagnitude() / 2;
}

Vector3::~Vector3()
{
}