#include "M33.h"

M33::M33() {
	rows[0] = V3(1.0f, 0.0f, 0.0f);
	rows[1] = V3(0.0f, 1.0f, 0.0f);
	rows[2] = V3(0.0f, 0.0f, 1.0f);
}

M33::M33(const V3& v1, const V3& v2, const V3& v3) {
	rows[0] = v1;
	rows[1] = v2;
	rows[2] = v3;
}

V3& M33::operator[](int i) {
	return rows[i];
}

V3 M33::GetColumn(int i) const {
	V3 ret(rows[0].xyz[i], rows[1].xyz[i], rows[2].xyz[i]);
	return ret;
}

void M33::SetColumn(int i, const V3& col) {
	rows[0][i] = col.x();
	rows[1][i] = col.y();
	rows[2][i] = col.z();
}

V3 M33::operator*(const V3& v) const {
	return V3(rows[0]*v, rows[1]*v, rows[2]*v);
}

M33 M33::operator*(const M33& m1) const {
	const M33 &m0 = *this;
	M33 ret;
	ret.SetColumn(0, m0*m1.GetColumn(0));
	ret.SetColumn(1, m0*m1.GetColumn(1));
	ret.SetColumn(2, m0*m1.GetColumn(2));
	return ret;
}

M33 M33::Inverted() {
	M33 ret;
	V3 a = GetColumn(0), b = GetColumn(1), c = GetColumn(2);
	V3 _a = b ^ c; _a = _a / (a * _a);
	V3 _b = c ^ a; _b = _b / (b * _b);
	V3 _c = a ^ b; _c = _c / (c * _c);
	ret[0] = _a;
	ret[1] = _b;
	ret[2] = _c;

	return ret;
}

M33 M33::Transpose() {
	M33 ret;
	ret[0] = GetColumn(0);
	ret[1] = GetColumn(1);
	ret[2] = GetColumn(2);

	return ret;
}

/**
 * Set this matrix as a rotation matrix about X axis by the specified degrees.
 *
 * @param angle		the rotation angle [degrees]
 */
void M33::SetRotationX(float angle) {
	float rad = DEG2RAD(angle);

	rows[0] = V3(1.0f, 0.0f, 0.0f);
	rows[1] = V3(0.0f, cosf(rad), -sinf(rad));
	rows[2] = V3(0.0f, sinf(rad), cosf(rad));
}

void M33::SetRotationY(float angle) {
	float rad = DEG2RAD(angle);

	rows[0] = V3(cosf(rad), 0.0f, sinf(rad));
	rows[1] = V3(0.0f, 1.0f, 0.0f);
	rows[2] = V3(-sinf(rad), 0.0f, cosf(rad));
}

void M33::SetRotationZ(float angle) {
	float rad = DEG2RAD(angle);

	rows[0] = V3(cosf(rad), -sin(rad), 0.0f);
	rows[1] = V3(sinf(rad), cosf(rad), 0.0f);
	rows[2] = V3(0.0f, 0.0f, 1.0f);
}

ostream& operator<<(ostream& ostr, const M33& m) {
	ostr << m.GetRow(0) << endl << m.GetRow(1) << endl  << m.GetRow(2) << endl;
	return ostr;
}

istream& operator>>(istream& istr, M33& m) {
	istr >> m[0] >> m[1] >> m[2];
	return istr;
}

V3 M33::GetRow(int i) const {
	return rows[i];
}

/**
 * Generate axes based on the first axis a.
 * The second and third axes are automatically generated,
 * and the generated axes are orthogonal to each other.
 *
 * @param axis	the first axis
 * @return		a matrix that represents the generated three axes in columns
 */
M33 M33::GenerateAxes(const V3& axis) {
	V3 a = axis.UnitVector();

	M33 ret;
	ret.SetColumn(0, a);

	V3 b;
	V3 xAxis(1.0f, 0.0f, 0.0f);
	V3 yAxis(0.0f, 1.0f, 0.0f);

	// Choose X axis or Y axis that is almost perpendicular to the given axis
	// to genearte the second axis b.
	if (fabs(a * xAxis) <= fabs(a * yAxis)) {
		b = xAxis ^ a;
	} else {
		b = yAxis ^ a;
	}

	V3 c = a ^ b;
	ret.SetColumn(1, b);
	ret.SetColumn(2, c);

	return ret;
}