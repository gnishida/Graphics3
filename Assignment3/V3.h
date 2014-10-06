#pragma once

#include <ostream>
#include <istream>

#define _USE_MATH_DEFINES
#include <math.h>

#define DEG2RAD(x)	((float)((x)/180.0f*M_PI))
#define RAD2DEG(x)	((float)((x)*180.0f/M_PI))

#define WHITE	0xFFFFFFFF
#define BLACK	0xFF000000
#define RED		0xFF0000FF
#define GREEN	0xFF00FF00
#define BLUE	0xFFFF0000

using namespace std;

class V3 {
friend class M33;

private:
	float xyz[3];

public:
	V3();
	V3(float x, float y, float z);
	float& operator[](int i);
	V3 operator+(const V3& v1) const;
	V3& operator+=(const V3& v1);
	V3 operator-(const V3& v1) const;
	V3& operator-=(const V3& v1);
	float operator*(const V3& v1) const;
	V3 operator^(const V3& v1) const;
	V3 operator*(float t) const;
	V3& operator*=(float t);
	V3 operator/(float t) const;
	V3& operator/=(float t);
	V3 UnitVector() const;
	float Length() const;
	V3 RotateAbout(const V3& axis, float angle, const V3& orig);
	V3 RotateAbout(const V3& dir, float angle);
	friend ostream& operator<<(ostream& ostr, const V3& v);
	friend istream& operator>>(istream& istr, V3& v);

	void SetColor(unsigned int color);
	unsigned int GetColor() const;

	V3 reflect(const V3& v) const;

	float x() const;
	float y() const;
	float z() const;
};

// axis aligned bounding box class
class AABB {
private:
	V3 corners[2];

public:
	AABB();
	AABB(const V3 &p);
	void AddPoint(const V3 &p);
	const V3& minCorner() const;
	const V3& maxCorner() const;
	V3 Size() const;
};