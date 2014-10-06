#include "Light.h"
#include "PPC.h"

Light::Light(const V3 &position, int type, float ambient, float diffuse, float specular) {
	this->position = position;
	this->type = type;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

/**
 * Rotate the light position/direction about the specified axis by the specified angle.
 *
 * @param axis		the axis about which the light position/direction rotates
 * @param angle		the angle by which the light position/direction rotates
 * @param orig		the ogin coordinate of the axis
 */
void Light::RotateAbout(const V3& axis, float angle, const V3& orig) {
	position = position.RotateAbout(axis, angle, orig);
}

/**
 * Get the color of the specified point with specified color according to this light source.
 */
V3 Light::GetColor(PPC* ppc, const V3 &p, const V3 &c, const V3 &n) const {
	V3 ret;

	if (type == TYPE_DIRECTIONAL_LIGHT) {
		ret = c * (ambient + diffuse * max(position.UnitVector() * n.UnitVector() * -1, 0.0f) + powf(max((p - ppc->C).reflect(n).UnitVector() * position.UnitVector() * -1.0f, 0.0f), specular));
	} else {
		ret = c * (ambient + diffuse * max((position - p).UnitVector() * n.UnitVector(), 0.0f) + powf(max((p - ppc->C).reflect(n).UnitVector() * (position - p).UnitVector(), 0.0f), specular));
	}

	if (ret.x() > 1.0f) ret[0] = 1.0f;
	if (ret.y() > 1.0f) ret[1] = 1.0f;
	if (ret.z() > 1.0f) ret[2] = 1.0f;

	return ret;
}