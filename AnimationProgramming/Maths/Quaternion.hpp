#pragma once
#include <string>

using namespace std;

class Quaternion
{
private:
	float w;
	float x;
	float y;
	float z;

public:
	Quaternion();
	Quaternion(float, float, float, float);

	void Mult(Quaternion);
	Quaternion Mult(Quaternion) const;

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

	float Norm() const;

	float Length() const;

	void Inverse();
	Quaternion Inverse() const;
	
	string ToString();
};

