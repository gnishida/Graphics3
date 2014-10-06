#include "v3.h"
#include "M33.h"
#include <limits>

#define SQR(x)	((x) * (x))

V3::V3() {
	xyz[0] = 0.0f;
	xyz[1] = 0.0f;
	xyz[2] = 0.0f;
}

V3::V3(float x, float y, float z) {
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;
}

float& V3::operator[](int i) {
	return xyz[i];
}

V3 V3::operator+(const V3& v1) const {
	V3 ret(xyz[0]+v1.x(), xyz[1]+v1.y(), xyz[2]+v1.z());
	return ret;
}

V3& V3::operator+=(const V3& v1) {
	xyz[0] += v1.x();
	xyz[1] += v1.y();
	xyz[2] += v1.z();
	return *this;
}

V3 V3::operator-(const V3& v1) const {
	V3 ret(xyz[0]-v1.x(), xyz[1]-v1.y(), xyz[2]-v1.z());
	return ret;
}

V3& V3::operator-=(const V3& v1) {
	xyz[0] -= v1.x();
	xyz[1] -= v1.y();
	xyz[2] -= v1.z();
	return *this;
}

float V3::operator*(const V3& v1) const {
	float ret = xyz[0]*v1.x() + xyz[1]*v1.y() + xyz[2]*v1.z();
	return ret;
}

V3 V3::operator^(const V3& v1) const {
	V3 ret(xyz[1]*v1.z() - xyz[2]*v1.y(), xyz[2]*v1.x() - xyz[0]*v1.z(), xyz[0]*v1.y() - xyz[1]*v1.x());
	return ret;
}

V3 V3::operator*(float t) const {
	V3 ret(xyz[0]*t, xyz[1]*t, xyz[2]*t);
	return ret;
}

V3& V3::operator*=(float t) {
	xyz[0] *= t;
	xyz[1] *= t;
	xyz[2] *= t;
	return *this;
}

V3 V3::operator/(float t) const {
	V3 ret(xyz[0]/t, xyz[1]/t, xyz[2]/t);
	return ret;
}

V3& V3::operator/=(float t) {
	xyz[0] /= t;
	xyz[1] /= t;
	xyz[2] /= t;
	return *this;
}

V3 V3::UnitVector() const {
	V3 ret = (*this) / Length();
	return ret;
}

float V3::Length() const {
	return sqrt(SQR(xyz[0]) + SQR(xyz[1]) + SQR(xyz[2]));
}

/**
 * Rotate this point about the specified axis by the specified angle.
 *
 * @param axis		the specified axis
 * @param angle		the specified angle
 * @param orig		the origin of the specified axis
 * @return			the rotated point
 */
V3 V3::RotateAbout(const V3& axis, float angle, const V3& orig) {
	M33 m = M33::GenerateAxes(axis);

	V3 pt2 = m.Inverted() * ((*this) - orig);
	M33 rot;
	rot.SetRotationX(angle);
	V3 pt3 = rot * pt2;
	return m * pt3 + orig;
}

/**
 * Rotate this vector about the specified direction by the specified angle.
 *
 * @param axis		the specified direction
 * @param angle		the specified angle
 * @return			the rotated point
 */
V3 V3::RotateAbout(const V3& dir, float angle) {
	return RotateAbout(dir, angle, V3(0, 0, 0));
	/*
	M33 m = M33::generateAxes(dir);

	V3 pt2 = m * (*this);
	V3 pt3 = M33::createRotationMatrixAboutX(angle) * pt2;
	return m.Inverted() * pt3;
	*/
}

ostream& operator<<(ostream& ostr, const V3& v) {
	ostr << v.x() << " " << v.y() << " "  << v.z(); 
	return ostr;
}

istream& operator>>(istream& istr, V3& v) {
	istr >> v.xyz[0] >> v.xyz[1] >> v.xyz[2];
	return istr;
}

void V3::SetColor(unsigned int color) {
	xyz[0] = ((float) (((unsigned char*)&color)[0])) / 255.0f;
	xyz[1] = ((float) (((unsigned char*)&color)[1])) / 255.0f;
	xyz[2] = ((float) (((unsigned char*)&color)[2])) / 255.0f;
}

unsigned int V3::GetColor() const {
  unsigned int ret = 0xFF000000;

  int red = (int) (xyz[0] * 255.0f);
  if (red > 255) red = 255;

  int green = (int) (xyz[1] * 255.0f);
  if (green > 255) green = 255;
  if (green < 0) green = 0;

  int blue = (int) (xyz[2] * 255.0f);
  if (blue > 255) blue = 255;
  if (blue < 0) blue = 0;

  return 0xFF000000 + red + (green << 8) + (blue << 16);
}

/**
 * Calculate the reflection vector.
 *
 * @param n		the normal vector
 * @return		the reflection vector
 */
V3 V3::reflect(const V3& n) const {
	return (*this) - n.UnitVector() * ((*this) * n.UnitVector()) * 2.0f;
}

float V3::x() const {
	return xyz[0];
}

float V3::y() const {
	return xyz[1];
}

float V3::z() const {
	return xyz[2];
}

AABB::AABB() {
	corners[0][0] = (numeric_limits<float>::max)();
	corners[0][1] = (numeric_limits<float>::max)();
	corners[0][2] = (numeric_limits<float>::max)();

	corners[1][0] = -(numeric_limits<float>::max)();
	corners[1][1] = -(numeric_limits<float>::max)();
	corners[1][2] = -(numeric_limits<float>::max)();
}

AABB::AABB(const V3 &p) {
	corners[0] = corners[1] = p;
}

void AABB::AddPoint(const V3 &p) {
	if (p.x() < corners[0].x()) {
		corners[0][0] = p.x();
	}
	if (p.y() < corners[0].y()) {
		corners[0][1] = p.y();
	}
	if (p.z() < corners[0].z()) {
		corners[0][2] = p.z();
	}

	if (p.x() > corners[1].x()) {
		corners[1][0] = p.x();
	}
	if (p.y() > corners[1].y()) {
		corners[1][1] = p.y();
	}
	if (p.z() > corners[1].z()) {
		corners[1][2] = p.z();
	}
}

const V3& AABB::minCorner() const {
	return corners[0];
}

const V3& AABB::maxCorner() const {
	return corners[1];
}

V3 AABB::Size() const {
	return corners[1] - corners[0];
}