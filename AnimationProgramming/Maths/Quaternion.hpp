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

	void Mult(Quaternion);
	Quaternion Mult(Quaternion) const;
	Quaternion operator*(const Quaternion q2) const
	{
		return Mult(q2);
	}

	void Add(Quaternion);
	Quaternion Add(Quaternion) const;

	void Neg(Quaternion);
	Quaternion Neg(Quaternion) const;

	void Scale(float);
	const Quaternion Scale(float) const;

	void Normalise();
	Quaternion Normalise() const;

	void Conjugate();
	const Quaternion Conjugate() const;

	float GetAngle() const
	{
		return std::acosf(w) * 2.f;
	}

	Vector3 GetAxis() const
	{
		const Vector3 xyz(x, y, z);
		return xyz / std::sinf(GetAngle() / 2.f);
	}

	Vector3 RotateVector(const Vector3& vec) const
	{
		//Rodrigues formula with quaternion is better than quat * vec * quat.getInverse()
		const float angle = GetAngle();
		const Vector3 unitAxis = GetAxis();

		const float cosAngle = std::cosf(angle);
		return cosAngle * vec + (1.f - cosAngle) * vec.dotProduct(unitAxis) * unitAxis + std::sin(angle) * unitAxis.cross(vec);
	}

	Matrix4 GetRotationMatrix() const
	{
		const float twoX = 2.f * x;
		const float twoY = 2.f * y;
		const float twoZ = 2.f * z;
		const float twoXX = twoX * x;
		const float twoXY = twoX * y;
		const float twoXZ = twoX * z;
		const float twoXW = twoX * w;
		const float twoYY = twoY * y;
		const float twoYZ = twoY * z;
		const float twoYW = twoY * w;
		const float twoZZ = twoZ * z;
		const float twoZW = twoZ * w;

		const Vector3 vec1(1.f - twoYY - twoZZ, twoXY + twoZW, twoXZ - twoYW);
		const Vector3 vec2(twoXY - twoZW, 1.f - twoXX - twoZZ, twoYZ + twoXW);
		const Vector3 vec3(twoXZ + twoYW, twoYZ - twoXW, 1.f - twoXX - twoYY);
		
		Matrix4 rst({   vec1.x, vec2.x, vec3.x, 0.f,
						vec1.y, vec2.y, vec3.y, 0.f,
						vec1.z, vec2.z, vec3.z, 0.f,
						0.f, 0.f, 0.f, 1.f});

		return rst / SquaredLength();
	}

	float Norm() const;

	float Length() const;
	float SquaredLength() const;

	void Inverse();
	Quaternion Inverse() const;
	
	string ToString();
};

