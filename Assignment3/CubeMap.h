#pragma once

#include "V3.h"
#include "Texture.h"
#include "TIFFImage.h"

class CubeMap {
public:
	Texture** textures;
	V3 center;

public:
	CubeMap(const TIFFImage &image);

	V3 lookup(const V3 &dir);
	void SetCenter(const V3 &center);
	void SetMipMap(int width, int height, float ds, float dt);
	int GetFace(const V3 &dir);
	V3 GetTextureCoord(int face, const V3 &dir);
};

