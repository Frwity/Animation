#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include "vector3.hpp"
#include "matrix.hpp"

using namespace std;

class Quaternion
{
public:

	float w;
	float x;
	float y;
	float z;

	Quaternion();
	Quaternion(float, float, float, float);

	void Mult(const Quaternion&);
	Quaternion Mult(const Quaternion&) const;

	Quaternion operator*(const Quaternion q2) const
	{
		return Mult(q2);
	}

	static float DotProduct(const Quaternion&, const Quaternion&);

	void Add(const Quaternion&);
	Quaternion Add(const Quaternion&) const;
	Quaternion operator+(const Quaternion& q2) const
	{
		return Add(q2);
	}

	void Neg(const Quaternion&);
	Quaternion Neg(const Quaternion&) const;

	void Scale(float);
	const Quaternion Scale(float) const;
	Quaternion operator*(const float f) const
	{
		return Scale(f);
	}
	Quaternion operator/(const float f) const
	{
		return Quaternion(w / f, x / f, y / f, z / f);
	}

	void Normalise();
	Quaternion Normalised() const;

	Quaternion operator-() const
	{
		return Quaternion(-w, -x, -y, -z);
	}

	void Conjugate();
	const Quaternion Conjugate() const;

	float GetAngle() const;

	Vector3 GetAxis() const;

	Vector3 RotateVector(const Vector3& vec) const;

	Matrix4 GetRotationMatrix() const;

	float Norm() const;

	float Length() const;
	float SquaredLength() const;

	void Inverse();
	Quaternion Inverse() const;

    static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
    static Quaternion SLerp(const Quaternion& q1, const Quaternion& q2, float t);
    static Quaternion NLerp(const Quaternion& q1, const Quaternion& q2, float t);

    string ToString() const;
};

