#pragma once

#include "V3.h"
#include <ostream>
#include <istream>

class M33 {
private:
	V3 rows[3];

public:
	M33();
	M33(const V3& v1, const V3& v2, const V3& v3);
	V3& operator[](int i);
	V3 GetColumn(int i) const;
	void SetColumn(int i, const V3& col);
	V3 operator*(const V3& v) const;
	M33 operator*(const M33& m) const;
	M33 Inverted();
	M33 Transpose();
	void SetRotationX(float angle);
	void SetRotationY(float angle);
	void SetRotationZ(float angle);
	friend ostream& operator<<(ostream& ostr, const M33& m);
	friend istream& operator>>(istream& istr, M33& m);

	V3 GetRow(int i) const;
	static M33 GenerateAxes(const V3& axis);
};