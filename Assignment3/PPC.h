#pragma once

#include "V3.h"
#include "M33.h"
#include <vector>

using namespace std;

class Frame;

class PPC {
public:
	/** The pixel width vector */
	V3 a;

	/** The pixel height vector */
	V3 b;

	/** The vector from the eye to the top left corner of the image */
	V3 c;

	/** The center of the eye */
	V3 C;

	/** The image width */
	int w;

	/** The image height */
	int h;

	/** The inverted camera matrix to facilitate the projection */
	M33 pMat;

public:
	PPC();
	PPC(float hfov,  int w, int h);
	//PPC(float hfov,  int w, int h, const V3& C, const V3& a, const V3& vd);
	void TranslateLR(float step);
	void TranslateUD(float step);
	void TranslateFB(float step);
	void Pan(float angle);
	void Tilt(float angle);
	void Roll(float angle);
	void SetFocalLength(float len);
	bool Project(const V3 &p, V3 &pp) const;
	PPC Interpolate(const PPC &ppc, float frac) const;
	void Save(char* filename);
	void Load(char* filename);

	void SetPMat();
	V3 GetVD() const;
	float GetHFOV() const;
	float GetFocalLength() const;
	void LookAt(const V3 &p, const V3 &vd, const V3 &up, float d);
	void Set(const V3& C, const V3& a, const V3& vd);
	void RotateAbout(const V3& axis, float angle, const V3& orig);
	void DrawPPCFrustum(PPC* ppc, Frame* fp, float scale) const;
};