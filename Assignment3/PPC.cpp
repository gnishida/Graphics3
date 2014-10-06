#include "PPC.h"
#include "M33.h"
#include "FrameBuffer.h"
#include <fstream>

PPC::PPC() {
	a = V3(1.0f, 0.0f, 0.0f);
	b = V3(0.0f, -1.0f, 0.0f);
	c = V3(0.0f, 0.0f, 0.0f);
	C = V3(0.0f, 0.0f, 0.0f);
	w = 0;
	h = 0;

	SetPMat();
}

PPC::PPC(float hfov,  int w, int h) : w(w), h(h) {
	a = V3(1.0f, 0.0f, 0.0f);
	b = V3(0.0f, -1.0f, 0.0f);
	C = V3(0.0f, 0.0f, 0.0f);
	float f = (float)w/2.0f/tanf(DEG2RAD(hfov/2.0f));
	c = V3(-(float)w/2.0f, (float)h/2.0f, -f);

	SetPMat();
}

/**
 * Translate the camera to the right by the specified step.
 * If the specified step is negative, the camera is translated to the left.
 *
 * @param step		the specified step
 */
void PPC::TranslateLR(float step) {
	C = C + a.UnitVector() * step;
}

/**
 * Translate the camera to upward by the specified step.
 * If the specified step is negative, the camera is translated to downward.
 *
 * @param step		the specified step
 */
void PPC::TranslateUD(float step) {
	C = C - b.UnitVector() * step;
}

void PPC::TranslateFB(float step) {
	C = C + GetVD() * step;
}

void PPC::Pan(float angle) {
	a = a.RotateAbout(b, -angle);
	c = c.RotateAbout(b, -angle);//, C);
	SetPMat();
}

void PPC::Tilt(float angle) {
	b = b.RotateAbout(a, angle);
	c = c.RotateAbout(a, angle);//, C);
	SetPMat();
}

void PPC::Roll(float angle) {
	a = a.RotateAbout(GetVD(), -angle);
	b = b.RotateAbout(GetVD(), -angle);
	c = c.RotateAbout(GetVD(), -angle);//, C);
	SetPMat();
}

/**
 * Change the focal length to the specified length.
 *
 * @param len		the new focal length
 */
void PPC::SetFocalLength(float len) {
	c = GetVD() * len - a * ((float)w/2.0f) - b * ((float)h/2.0f);
	SetPMat();
}

/**
 * Project the 3D point p onto the 2D point on the image screen.
 * The z element of the pp represents the scale factor along the Z axis.
 * The negative value of this means the 3D point is behind the camera.
 *
 * @param p		3D point to be projected
 * @param pp	2D point on the image
 * @return		true if the 3D point is in front of the camera; false if it is behind the camera
 */
bool PPC::Project(const V3 &p, V3 &pp) const {
	pp[0] = pp[1] = pp[2] = std::numeric_limits<float>::max();

	V3 q = pMat * (p - C);

	if (q[2] < 0.0f) return false;

	pp[0] = q[0] / q[2];
	pp[1] = q[1] / q[2];
	pp[2] = 1.0f / q[2];

	return true;
}

/**
 * Interpolate between two cameras and generate N intermediate cameras.
 *
 * @param ppc		another camera
 * @param N			the number of generated cameras
 * @param cameras	the generated cameras
 */
PPC PPC::Interpolate(const PPC &ppc, float frac) const {
	PPC ret = *this;

	V3 vd = GetVD();
	
	V3 newC = C + (ppc.C - C) * frac;
	V3 newvd = (vd + (ppc.GetVD() - vd) * frac).UnitVector();
	V3 newa = (a + (ppc.a - a) * frac).UnitVector() * a.Length();

	ret.Set(newC, newa, newvd);

	return ret;//PPC(GetHFOV(), w, h, newC, newa, newvd);
}

/**
 * Save the camera object into the specified file.
 * All the data will be stored in the file with a space as a delimiter.
 *
 * @param filename		the specified file name
 */
void PPC::Save(char* filename) {
	ofstream ofs(filename);
	ofs << a << " " << b << " " << c << " " << C << " " << w << " " << h;
}

/**
 * Load the camera object from the specified file.
 * All the data are stored in the file with a space as a delimiter.
 *
 * @param filename		the specified file name
 */
void PPC::Load(char* filename) {
	ifstream ifs(filename);
	ifs >> a >> b >> c >> C >> w >> h;
	SetPMat();
}

/**
 * Calculate the inverted camera matrix.
 * This function needs to be called every time when the vector a, b, or c is updated.
 */
void PPC::SetPMat() {
	M33 cam;
	cam.SetColumn(0, a);
	cam.SetColumn(1, b);
	cam.SetColumn(2, c);
	pMat = cam.Inverted();
}

/**
 * Get the normalized view direction.
 *
 * @return		the normalized view direction
 */
V3 PPC::GetVD() const {
	return (a^b).UnitVector();
}

/**
 * Get the horizontal field of view [degree].
 *
 * @return		the horizontal field of view [degree]
 */
float PPC::GetHFOV() const {
	return RAD2DEG(2.0f * atan2f((float)w / 2.0f * a.Length(), GetFocalLength()));
}

float PPC::GetFocalLength() const {
	return GetVD() * c;
}

/**
 * Setup this camera such that it looks at the specified point with the specified view direction, up direction, and the distance from the point.
 *
 * @param p		the target point that this camera will look at
 * @param vd	the view direction
 * @param up	the up direction
 * @param d		the distance between the center of this camera and the target
 */
void PPC::LookAt(const V3 &p, const V3 &vd, const V3 &up, float d) {
	float f = GetFocalLength();

	C = p - vd.UnitVector() * d;
	a = (vd ^ up).UnitVector() * a.Length();
	b = (vd ^ a).UnitVector() * b.Length();
	c = vd.UnitVector() * f - a * ((float)w / 2.0f) - b * ((float)h / 2.0f);

	SetPMat();
}

/**
 * Setup this camera according to the specified center, vector a, and the view direction.
 *
 * @param C		the center of the camera
 * @param a		the vector a (The horizontal direction of the screen)
 * @param vd	the view direction of the camera
 */
void PPC::Set(const V3& C, const V3& a, const V3& vd) {
	this->C = C;
	this->a = a;

	b = (vd ^ a).UnitVector() * b.Length();

	c = vd.UnitVector() * GetFocalLength() - a * ((float)w/2.0f) - b * ((float)h/2.0f);

	SetPMat();
}

/**
 * Rotate the camera around the specified axis and the specified origin by the specified angle.
 *
 * @param axis		the axis 
 * @param angle		the angle
 * @param orig		the origin
 */
void PPC::RotateAbout(const V3& axis, float angle, const V3& orig) {
	a = a.RotateAbout(axis, angle);
	b = b.RotateAbout(axis, angle);
	c = c.RotateAbout(axis, angle);
	C = C.RotateAbout(axis, angle, orig);

	SetPMat();
}

/**
 * Draw a frustum of this camera.
 *
 * @param ppc		the current camera
 */
void PPC::DrawPPCFrustum(PPC* ppc, Frame* fp, float scale) const {
	V3 color(0.0f, 1.0f, 1.0f);
	
	fp->Draw3DBigPoint(ppc, C, 4, V3(0.0f, 0.0f, 0.0f));

	fp->Draw3DSegment(ppc, C, color, C + c * scale, color);
	fp->Draw3DSegment(ppc, C, color, C + (c + a * w) * scale, color);
	fp->Draw3DSegment(ppc, C, color, C + (c + b * h) * scale, color);
	fp->Draw3DSegment(ppc, C, color, C + (c + a * w + b * h) * scale, color);

	fp->Draw3DSegment(ppc, C + c * scale, color, C + (c + a * w) * scale, color);
	fp->Draw3DSegment(ppc, C + (c + a * w) * scale, color, C + (c + a * w + b * h) * scale, color);
	fp->Draw3DSegment(ppc, C + (c + a * w + b * h) * scale, color, C + (c + b * h) * scale, color);
	fp->Draw3DSegment(ppc, C + (c + b * h) * scale, color, C + c * scale, color);
}