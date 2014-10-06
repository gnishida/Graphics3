#include "CubeMap.h"
#include <assert.h>

CubeMap::CubeMap(const TIFFImage &image) {
	textures = new Texture*[6];

	int len;
	if (image.width() < image.height()) {
		len = image.width() / 3;

		textures[0] = new Texture(image, len, len*3, len, len);
		textures[1] = new Texture(image, 0, len*2, len, len);
		textures[2] = new Texture(image, len, len*2, len, len);
		textures[3] = new Texture(image, len*2, len*2, len, len);
		textures[4] = new Texture(image, len, len, len, len);
		textures[5] = new Texture(image, len, 0, len, len);
	} else {
		len = image.width() / 4;

		textures[0] = new Texture(image, len, len*2, len, len);
		textures[1] = new Texture(image, 0, len, len, len);
		textures[2] = new Texture(image, len, len, len, len);
		textures[3] = new Texture(image, len*2, len, len, len);
		textures[4] = new Texture(image, len, 0, len, len);
		textures[5] = new Texture(image, len*3, len, len, len);		// should be flip vertically
	}
}

V3 CubeMap::lookup(const V3 &dir) {
	V3 ret;
	
	V3 d = dir.UnitVector();
	
	int face = GetFace(dir);
	V3 coord = GetTextureCoord(face, dir);

	return textures[face]->GetColor(coord.x(), coord.y());
}

void CubeMap::SetCenter(const V3 &center) {
	this->center = center;
}

void CubeMap::SetMipMap(int width, int height, float ds, float dt) {
	textures[0]->SetMipMap(width, height, ds, dt);
	for (int i = 1; i < 6; i++) {
		textures[i]->mipmap_id1 = textures[0]->mipmap_id1;
		textures[i]->mipmap_id2 = textures[0]->mipmap_id2;
		textures[i]->mipmap_s = textures[0]->mipmap_s;
	}
}

int CubeMap::GetFace(const V3 &d) {
	if (fabs(d.x()) >= fabs(d.y()) && fabs(d.x()) >= fabs(d.z())) {
		if (d.x() >= 0) {
			return 3;
		} else {
			return 1;
		}
	} else if (fabs(d.y()) >= fabs(d.z())) {
		if (d.y() >= 0) {
			return 0;
		} else {
			return 4;
		}
	} else {
		if (d.z() >= 0) {
			return 2;
		} else {
			return 5;
		}
	}
}

V3 CubeMap::GetTextureCoord(int face, const V3 &dir) {
	V3 d = dir.UnitVector();
	V3 ret;

	if (face == 3) {
		ret[0] = 0.5f - d.z() / d.x() / 2.0f;
		ret[1] = 0.5f + d.y() / d.x() / 2.0f;
	} else if (face == 1) {
		ret[0] = 0.5f - d.z() / d.x() / 2.0f;
		ret[1] = 0.5f - d.y() / d.x() / 2.0f;
	} else if (face == 0) {
		ret[0] = 0.5f + d.x() / d.y() / 2.0f;
		ret[1] = 0.5f - d.z() / d.y() / 2.0f;
	} else if (face == 4) {
		ret[0] = 0.5f - d.x() / d.y() / 2.0f;
		ret[1] = 0.5f - d.z() / d.y() / 2.0f;
	} else if (face == 2) {
		ret[0] = 0.5f + d.x() / d.z() / 2.0f;
		ret[1] = 0.5f + d.y() / d.z() / 2.0f;
	} else if (face == 5) {
		ret[0] = 0.5f - d.x() / d.z() / 2.0f;
		ret[1] = 0.5f + d.y() / d.z() / 2.0f;
	}

	assert(ret[0] >= 0.0f && ret[0] <= 1.0f);
	assert(ret[1] >= 0.0f && ret[1] <= 1.0f);

	return ret;
}
