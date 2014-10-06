#pragma once

#include "V3.h"

class PPC;

class Light {
public:
	enum { TYPE_DIRECTIONAL_LIGHT = 0, TYPE_POINT_LIGHT };
private:
	/** For the directional light, this is the direction. For the point light, this is the position. */
	V3 position;

	/** directional light or point light */
	int type;

	/** ambient coefficient, typically 0.4 */
	float ambient;

	/** diffuse coefficient, typically 0.6 */
	float diffuse;

	/** specular coefficient, typeically [40, 100] */
	float specular;

public:
	Light(const V3 &position, int type, float ambient, float diffuse, float specular);

	void RotateAbout(const V3& axis, float angle, const V3& orig);
	V3 GetColor(PPC* ppc, const V3 &p, const V3 &c, const V3 &n) const;
};

