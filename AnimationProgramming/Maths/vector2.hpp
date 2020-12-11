#ifndef __VECTOR2_HPP__
#define __VECTOR2_HPP__

#include <math.h>

class Vector2
{
private:
    
public:
    float x, y;

    Vector2();
    Vector2(float _x, float _y);
    ~Vector2();

    Vector2 normalized() const
    {
        Vector2 to_return;
        
        float magnitude = sqrtf(x * x + y * y);

        to_return.x = x / magnitude;
        to_return.y = y / magnitude;

        return to_return;
    }
};


#endif