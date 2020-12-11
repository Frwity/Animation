#include <string>
#include <iostream>
#include <cassert>

#include <math.h>
#include "Core/Maths/matrix.hpp"

Matrix4::Matrix4()
{
    for (int i = 0; i < 16; ++i)
        this->matrix[i] = 0;
}

Matrix4::Matrix4(std::initializer_list<float> matrix)
{
    assert(matrix.size() == 16);

    int i = 0;
    for (float f : matrix)
        this->matrix[i++] = f;
}

Matrix4::Matrix4(const Matrix4 &_matrix)
{
    for (int i = 0; i < 16; ++i)
        this->matrix[i] = _matrix.matrix[i];
}

Matrix4::~Matrix4()
{
}

float Matrix4::accessor(int c, int l) const
{
    return this->matrix[c + l * 4];
}

float &Matrix4::accessor(int c, int l)
{
    return this->matrix[c + l * 4];
}

void Matrix4::display() const
{
    for (int i = 0; i < 4; ++i)
    {
        printf("[");
        for (int j = 0; j < 4; ++j)
        {
            printf(" %f", Matrix4::accessor(j, i));
            if (j < 4 - 1)
                printf(" \t|");
        }
        printf("]\n");
    }
}

Matrix4 Matrix4::identity()
{
    Matrix4 identity;

    for (int i = 0; i < 4; ++i)
        identity.matrix[i * 4 + i] = 1;

    return identity;
}

Matrix4 Matrix4::transpose() const
{
    Matrix4 to_return;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            to_return.accessor(j, i) = Matrix4::accessor(i, j);
    //TODO ?
    return to_return;
}

Matrix4 Matrix4::operator+(const Matrix4 &to_add) const
{
    Matrix4 to_return;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            to_return.accessor(j, i) = this->accessor(i, j) + to_add.accessor(i, j);

    return to_return;
}

Matrix4 Matrix4::operator-(const Matrix4 &to_del) const
{
    Matrix4 to_return;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            to_return.accessor(j, i) = this->accessor(i, j) - to_del.accessor(i, j);

    return to_return;
}

Matrix4 Matrix4::operator*(const Matrix4 &to_mult) const
{
    Matrix4 to_return;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                to_return.accessor(j, i) += this->accessor(k, i) * to_mult.accessor(j, k);

    return to_return;
}

Vector4 Matrix4::operator*(const Vector4 &to_mult) const
{
    Vector4 to_return;

    to_return.x = to_mult.x * this->accessor(0, 0) + to_mult.y * this->accessor(1, 0) + to_mult.z * this->accessor(2, 0) + to_mult.w * this->accessor(3, 0);

    to_return.y = to_mult.x * this->accessor(0, 1) + to_mult.y * this->accessor(1, 1) + to_mult.z * this->accessor(2, 1) + to_mult.w * this->accessor(3, 1);

    to_return.z = to_mult.x * this->accessor(0, 2) + to_mult.y * this->accessor(1, 2) + to_mult.z * this->accessor(2, 2) + to_mult.w * this->accessor(3, 2);

    to_return.w = to_mult.x * this->accessor(0, 3) + to_mult.y * this->accessor(1, 3) + to_mult.z * this->accessor(2, 3) + to_mult.w * this->accessor(3, 3);

    return to_return;
}

Vector3 Matrix4::operator*(const Vector3 &to_mult) const
{
    Vector4 temp;

    temp.x = to_mult.x * this->accessor(0, 0) + to_mult.y * this->accessor(1, 0) + to_mult.z * this->accessor(2, 0);

    temp.y = to_mult.x * this->accessor(0, 1) + to_mult.y * this->accessor(1, 1) + to_mult.z * this->accessor(2, 1);

    temp.z = to_mult.x * this->accessor(0, 2) + to_mult.y * this->accessor(1, 2) + to_mult.z * this->accessor(2, 2);

    temp.w = to_mult.x * this->accessor(0, 3) + to_mult.y * this->accessor(1, 3) + to_mult.z * this->accessor(2, 3);

    Vector3 to_return(temp.x, temp.y, temp.z);

    return to_return;
}

bool Matrix4::operator==(const Matrix4 &toCompare) const
{
    for (int i = 0; i < 16; ++i)
        if (this->matrix[i] != toCompare.matrix[i])
            return 0;

    return 1;
}

bool Matrix4::operator!=(const Matrix4 &toCompare) const
{
    int count = 0;

    for (int i = 0; i < 16; ++i)
        if (this->matrix[i] == toCompare.matrix[i])
            count++;

    return 1;
}

float Matrix4::det() const
{
    return matrix[0] *
               (matrix[5] * (matrix[10] * matrix[15] - matrix[14] * matrix[11]) - matrix[6] * (matrix[9] * matrix[15] - matrix[13] * matrix[11]) + matrix[7] * (matrix[9] * matrix[14] - matrix[13] * matrix[10])) -
           matrix[1] *
               (matrix[4] * (matrix[10] * matrix[15] - matrix[14] * matrix[11]) - matrix[6] * (matrix[8] * matrix[15] - matrix[12] * matrix[11]) + matrix[7] * (matrix[8] * matrix[14] - matrix[12] * matrix[10])) +
           matrix[2] *
               (matrix[4] * (matrix[9] * matrix[15] - matrix[13] * matrix[11]) - matrix[5] * (matrix[8] * matrix[15] - matrix[12] * matrix[11]) + matrix[7] * (matrix[8] * matrix[13] - matrix[12] * matrix[9])) -
           matrix[3] *
               (matrix[4] * (matrix[9] * matrix[14] - matrix[13] * matrix[10]) - matrix[5] * (matrix[8] * matrix[14] - matrix[12] * matrix[10]) + matrix[6] * (matrix[8] * matrix[13] - matrix[12] * matrix[9]));
}

Matrix4 Matrix4::GetSubMatrix(const Matrix4 &_matrix, int excludedColumnIndex, int excludedLineIndex) const
{
    int j2 = 0;
    int i2 = 0;

    Matrix4 to_return;

    for (int i = 0; i < 4; ++i)
    {
        if (i == excludedLineIndex)
            continue;
        j2 = 0;
        for (int j = 0; j < 4; ++j)
        {
            if (j == excludedColumnIndex)
                continue;
            to_return.accessor(j2++, i2) = _matrix.accessor(j, i);
        }
        i2++;
    }

    return to_return;
}

bool Matrix4::isOrtho() const
{
    if ((*this * this->transpose()) == Matrix4::identity())
        return 1;

    return 0;
}

float Matrix4::minorCoef(int excludedColumnIndex, int excludedLineIndex) const
{
    return GetSubMatrix(*this, excludedColumnIndex, excludedLineIndex).det();
}

float Matrix4::coFactor(int excludedColumnIndex, int excludedLineIndex) const
{
    return minorCoef(excludedColumnIndex, excludedLineIndex) * pow(-1, excludedColumnIndex + excludedLineIndex + 2);
}

Matrix4 Matrix4::coMatrix() const
{
    Matrix4 to_return;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            to_return.accessor(j, i) = this->coFactor(j, i);

    return to_return;
}

Matrix4 Matrix4::adjunct() const
{
    return coMatrix().transpose();
}

Matrix4 Matrix4::reverse() const
{
    assert(this->det() != 0);

    Matrix4 to_return(this->adjunct());
    float det = this->det();

    for (int i = 0; i < 16; ++i)
        to_return.matrix[i] /= det;

    return to_return;
}

Matrix4 Matrix4::resolve(const Matrix4 &right)
{
    return reverse() * right.transpose();
}

Matrix4 Matrix4::createScaleMatrix(Vector3 scale)
{
    Matrix4 matrix;

    matrix.accessor(0, 0) = scale.x;
    matrix.accessor(1, 1) = scale.y;
    matrix.accessor(2, 2) = scale.z;
    matrix.accessor(3, 3) = 1;

    return matrix;
}

Matrix4 Matrix4::createTranslationMatrix(Vector3 translate)
{
    Matrix4 matrix;

    matrix.accessor(0, 0) = 1;
    matrix.accessor(1, 1) = 1;
    matrix.accessor(2, 2) = 1;
    matrix.accessor(3, 3) = 1;
    matrix.accessor(3, 0) = translate.x;
    matrix.accessor(3, 1) = translate.y;
    matrix.accessor(3, 2) = translate.z;

    return matrix;
}

Matrix4 Matrix4::createXRotationMatrix(float angleX)
{
    Matrix4 matrix;

    matrix.accessor(0, 0) = 1;
    matrix.accessor(1, 1) = cos(angleX);
    matrix.accessor(1, 2) = -sin(angleX);
    matrix.accessor(2, 1) = sin(angleX);
    matrix.accessor(2, 2) = cos(angleX);
    matrix.accessor(3, 3) = 1;

    return matrix;
}

Matrix4 Matrix4::createYRotationMatrix(float angleY)
{
    Matrix4 matrix;

    matrix.accessor(0, 0) = cos(angleY);
    matrix.accessor(2, 0) = sin(angleY);
    matrix.accessor(0, 2) = -sin(angleY);
    matrix.accessor(2, 2) = cos(angleY);
    matrix.accessor(1, 1) = 1;
    matrix.accessor(3, 3) = 1;

    return matrix;
}

Matrix4 Matrix4::createZRotationMatrix(float angleZ)
{
    Matrix4 matrix;

    matrix.accessor(0, 0) = cos(angleZ);
    matrix.accessor(1, 0) = -sin(angleZ);
    matrix.accessor(0, 1) = sin(angleZ);
    matrix.accessor(1, 1) = cos(angleZ);
    matrix.accessor(2, 2) = 1;
    matrix.accessor(3, 3) = 1;

    return matrix;
}

Matrix4 Matrix4::createFixedAngleEulerRotationMatrix(float angleX, float angleY, float angleZ)
{
    return createYRotationMatrix(angleY) * createXRotationMatrix(angleX) * createZRotationMatrix(angleZ);
}

Matrix4 Matrix4::createTRSMatrix(Vector3 translate, Vector3 rotate, Vector3 scale)
{
    return createTranslationMatrix(translate) * createFixedAngleEulerRotationMatrix(rotate.x, rotate.y, rotate.z) * createScaleMatrix(scale);
}

Matrix4 Matrix4::createRotationMatrix(const Vector3 vect, float angle)
{
    Matrix4 rotationMatrix = identity();

    float c = cos(angle);
    float s = sin(angle);
    float t = 1 - cos(angle);

    rotationMatrix.accessor(0, 0) = (t * vect.x) * (t * vect.x) + c;
    rotationMatrix.accessor(0, 1) = t * vect.x * vect.y - s * vect.z;
    rotationMatrix.accessor(0, 2) = t * vect.x * vect.z + s * vect.y;

    rotationMatrix.accessor(1, 0) = t * vect.x * vect.y + s * vect.z;
    rotationMatrix.accessor(1, 1) = (t * vect.y) * (t * vect.y) + c;
    rotationMatrix.accessor(1, 2) = t * vect.y * vect.z - s * vect.x;
    
    rotationMatrix.accessor(2, 0) = t * vect.z * vect.x - s * vect.y;
    rotationMatrix.accessor(2, 1) = t * vect.y * vect.z + s * vect.x;
    rotationMatrix.accessor(2, 2) = (t * vect.z) * (t * vect.z) + c;

    return rotationMatrix;
}

Matrix4 Matrix4::createProjectionMatrix(float d)
{
    Matrix4 projectionMatrix;

    projectionMatrix.accessor(0,0) = 1;
    projectionMatrix.accessor(1,1) = 1;
    projectionMatrix.accessor(2,2) = 1;
    projectionMatrix.accessor(3,2) = 1 / d;

    return projectionMatrix;
}

Matrix4 Matrix4::createPerspectiveProjectionMatrix(int width, int height, float near, float far, float fov)
{
    float temp1, temp2, temp3, temp4;
    {
        float ymax, xmax;
        ymax = near * tanf(fov * M_PI / 360.0f);
        xmax = ymax * (width / height);

        temp1 = 2.0f * near;
        temp2 = 2.0f * xmax;
        temp3 = 2.0f * ymax;
        temp4 = far - near;
    }
    Matrix4 projectionMatrix = {temp1 / temp2, 0.0f, 0.0f, 0.0f,
                                0.0f, temp1 / temp3, 0.0f, 0.0f,
                                0.0f, 0.0f, (-far - near) / temp4, (-temp1 * far) / temp4,
                                0.0f, 0.0f, -1.0f, 0.0f};

    return projectionMatrix;
}

Matrix4 Matrix4::createOrthoProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
{
    Matrix4 Ortho;
    
    Ortho.accessor(0, 0) = (float)(2) / (right - left);
    Ortho.accessor(1, 1) = (float)(2) / (top - bottom);
    Ortho.accessor(3, 0) = - (right + left) / (right - left);
    Ortho.accessor(3, 1) = - (top + bottom) / (top - bottom);

    Ortho.accessor(2, 2) = - (float)(2) / (zFar - zNear);
    Ortho.accessor(3, 2) = - zNear / (zFar - zNear);

    Ortho.accessor(3, 3) = 1; 

    return Ortho;
}

Matrix4 Matrix4::createRSTMatrix(Vector3 translate, Vector3 rotate, Vector3 scale)
{
    return (createXRotationMatrix(rotate.x) * createZRotationMatrix(rotate.y) * createYRotationMatrix(rotate.z)) * createScaleMatrix(scale) * createTranslationMatrix(translate);
}

Matrix4 Matrix4::lookAt(const Vector3 eye, const Vector3 target, const Vector3 tmp)
{
    const Vector3 forward = (target - eye).normalized(); 
    const Vector3 right = (forward.vectorProduct(tmp)).normalized(); 
    const Vector3 up = right.vectorProduct(forward); 
 
    Matrix4 cameraMatrix; 
 
    cameraMatrix.accessor(0, 0) = right.x; 
    cameraMatrix.accessor(1, 0) = right.y; 
    cameraMatrix.accessor(2, 0) = right.z;
    cameraMatrix.accessor(0, 1) = up.x; 
    cameraMatrix.accessor(1, 1) = up.y; 
    cameraMatrix.accessor(2, 1) = up.z; 
    cameraMatrix.accessor(0, 2) = -forward.x; 
    cameraMatrix.accessor(1, 2) = -forward.y; 
    cameraMatrix.accessor(2, 2) = -forward.z; 
 
    cameraMatrix.accessor(3, 0) = -right.dotProduct(eye); 
    cameraMatrix.accessor(3, 1) = -up.dotProduct(eye); 
    cameraMatrix.accessor(3, 2) = forward.dotProduct(eye); 

    cameraMatrix.accessor(3, 3) = 1;
 
    return cameraMatrix;
}

void Matrix4::move(Vector3 translate, Vector3 rotate, Vector3 scale)
{
    *this = *this * Matrix4::createTRSMatrix(translate, rotate, scale);
}

void Matrix4::move(Matrix4 transformation)
{
    *this = *this * transformation;
}