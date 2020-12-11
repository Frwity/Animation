#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <initializer_list>

#include "Core/Maths/vector3.hpp"
#include "Core/Maths/vector4.hpp"

class Matrix4
{
protected:
    float matrix[16];
public:
    Matrix4();
    Matrix4(std::initializer_list<float> matrix);
    Matrix4(const Matrix4&);
    ~Matrix4();

    float accessor(int, int) const;
    float& accessor(int, int);

    void display() const;
    static Matrix4 identity();

    Matrix4 transpose() const;

    Matrix4 operator+(const Matrix4&) const;
    Matrix4 operator-(const Matrix4&) const;
    Matrix4 operator*(const Matrix4&) const;
    Vector4 operator*(const Vector4&) const;
    Vector3 operator*(const Vector3&) const;
    bool operator==(const Matrix4&) const;
    bool operator!=(const Matrix4&) const;

    bool isOrtho() const;
    float det() const;

    Matrix4 GetSubMatrix(const Matrix4& matrix, int excludedColumnIndex, int excludedLineIndex) const;

    float minorCoef(int excludedColumnIndex, int excludedLineIndex) const;
    float coFactor(int excludedColumnIndex, int excludedLineIndex) const;
    Matrix4 coMatrix() const;
    Matrix4 adjunct() const;
    Matrix4 reverse() const;
    Matrix4 resolve(const Matrix4& right);

    static Matrix4 createScaleMatrix(Vector3 scale);

    static Matrix4 createTranslationMatrix(Vector3 translate);

    static Matrix4 createXRotationMatrix(float angleX);
    static Matrix4 createYRotationMatrix(float angleY);
    static Matrix4 createZRotationMatrix(float angleZ);
    static Matrix4 createFixedAngleEulerRotationMatrix(float angleX, float angleY, float angleZ); 

    static Matrix4 createTRSMatrix(Vector3 translate, Vector3 rotate, Vector3 scale);

    static Matrix4 createRotationMatrix(const Vector3 vect, float angle);

    static Matrix4 createProjectionMatrix(float d);

    static Matrix4 createPerspectiveProjectionMatrix(int width, int height, float near, float far, float fov);

    static Matrix4 createOrthoProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar);


    static Matrix4 createRSTMatrix(Vector3 translate, Vector3 rotate, Vector3 scale); // test

    static Matrix4 lookAt(const Vector3 eye, const Vector3 target, const Vector3 tmp);

    void move(Vector3 translate, Vector3 rotate, Vector3 scale);
    void move(Matrix4 transformation);

};

#endif