#include "Quaternion.hpp"

using namespace std;

Quaternion::Quaternion()
{
	w = x = y = z = 0;
}

Quaternion::Quaternion(float _w, float _x, float _y, float _z)
{
	w = _w;
	x = _x;
	y = _y;
	z = _z;
}

void Quaternion::Mult(Quaternion q2)
{
	float tempw = w;
	float tempx = x;
	float tempy = y;
	float tempz = z;

	w = -tempw * q2.x - tempy * q2.y - tempz * q2.z + tempw * q2.w;
	x = tempw * q2.w + tempy * q2.z - tempz * q2.y + tempw * q2.x;
	y = -tempw * q2.z + tempy * q2.w + tempz * q2.x + tempw * q2.y;
	z = tempw * q2.y - tempy * q2.x + tempz * q2.w + tempw * q2.z;
}

Quaternion Quaternion::Mult(Quaternion q2) const
{
	return Quaternion(	-x * q2.x - y * q2.y - z * q2.z + w * q2.w,
						x * q2.w + y * q2.z - z * q2.y + w * q2.x,
						-x * q2.z + y * q2.w + z * q2.x + w * q2.y,
						x * q2.y - y * q2.x + z * q2.w + w * q2.z);
}

void Quaternion::Add(Quaternion q2)
{
	x = x + q2.x;
	y = y + q2.y;
	z = z + q2.z;
	w = w + q2.w;
}

Quaternion Quaternion::Add(Quaternion q2) const
{
	return Quaternion(w + q2.w, x + q2.x, y + q2.y, z + q2.z);
}

void Quaternion::Neg(Quaternion q2)
{
	x = x - q2.x;
	y = y - q2.y;
	z = z - q2.z;
	w = w - q2.w;
}

Quaternion Quaternion::Neg(Quaternion q2) const
{
	return Quaternion(w - q2.w, x - q2.x, y - q2.y, z - q2.z);
}

void Quaternion::Scale(float s)
{
	w *= s;
	x *= s;
	y *= s;
	z *= s;
}

const Quaternion Quaternion::Scale(float s) const
{
	return Quaternion(w * s, x * s, y * s, z * s);
}

void Quaternion::Normalise()
{
	float u = sqrtf(x * x + y * y + z * z + w * w);
	x /= u;
	y /= u;
	z /= u;
	w /= u;
}

Quaternion Quaternion::Normalise() const
{
	float u = sqrtf(x * x + y * y + z * z + w * w);
	return Quaternion(x / u, y / u, z / u,	w / u);
}

void Quaternion::Conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

const Quaternion Quaternion::Conjugate() const
{
	return Quaternion(w, -x, -y, -z);
}

float Quaternion::Norm() const
{
	return w * w + x * x + y * y + z * z;
}

float Quaternion::Length() const
{
	return sqrtf(Norm());
}

float Quaternion::SquaredLength() const
{
	return Norm();
}

void Quaternion::Inverse()
{
	Conjugate();
	Scale(1 / Norm());
}

Quaternion Quaternion::Inverse() const
{
	return Conjugate().Scale(1 / Norm());
}


string Quaternion::ToString()
{
	return "[ " + to_string(w) + " ; " + to_string(x) + " ; " + to_string(y) + " ; " + to_string(z) + " ]";
}