#pragma once

#include "V3.h"
#include "TIFFImage.h"
#include <vector>

class Texture {
private:
	std::vector<int> widths;
	std::vector<int> heights;
	std::vector<unsigned int*> images;

public:
	int mipmap_id1;
	int mipmap_id2;
	float mipmap_s;

public:
	Texture();
	Texture::Texture(const TIFFImage &image, int x0, int y0, int w, int h);
	Texture(const char* filename);
	~Texture();

	V3 GetColor(float s, float t);
	void SetColor(int u, int v, unsigned int c);
	void SetMipMap(int width, int height, float ds, float dt);
	void CreateMipMap(int width, int height);
	void CreateMipMap(int width, int height, const TIFFImage &src, int x0, int y0);

private:
	V3 GetColor(unsigned int* image, int width, int height, float u, float v);
};

