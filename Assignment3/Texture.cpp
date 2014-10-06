#include "Texture.h"
#include "scene.h"
#include <libtiff/tiffio.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#define log2f(x)		(logf(x) / logf(2.0f))

Texture::Texture() {
	mipmap_id1 = 0;
	mipmap_id2 = 0;
	mipmap_s = 1.0f;
}

Texture::Texture(const TIFFImage &src, int x0, int y0, int w, int h) {
	unsigned int* image = (unsigned int*)_TIFFmalloc(sizeof(unsigned int) * w * h);
	widths.push_back(w);
	heights.push_back(h);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			image[x + y * w] = src.pix[x + x0 + (y + y0) * src.width()]; 
		}
	}

	images.push_back(image);

	CreateMipMap(w, h, src, x0, y0);
}

Texture::Texture(const char* filename) {
	TIFF* tiff = TIFFOpen(filename, "r");
	if (tiff == NULL) throw "File is not accessible.";

	int w, h;

	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &h);

	unsigned int* image = (unsigned int*)_TIFFmalloc(sizeof(unsigned int) * w * h);
	widths.push_back(w);
	heights.push_back(h);
	images.push_back(image);
	if (!TIFFReadRGBAImage(tiff, w, h, image, 0)) {
		delete [] image;
		image = NULL;
		TIFFClose(tiff);
		return;
	}

	TIFFClose(tiff);

	CreateMipMap(w, h);
}

Texture::~Texture() {
	for (int i = 0; i < images.size(); i++) {
		delete [] images[i];
	}
}

/**
 * Get the color at the specified texel (s, t) of this image.
 *
 * @param u		the x coordinate (0.0 - 1.0)
 * @param v		the y coordinate (0.0 - 1.0)
 * @return		the color
 */
V3 Texture::GetColor(float s, float t) {
	if (images.size() == 0) return V3(0.0f, 0.0f, 0.0f);

	// tri-linear interpolation
	V3 c1 = GetColor(images[mipmap_id1], widths[mipmap_id1], heights[mipmap_id1], s, t);
	V3 c2 = GetColor(images[mipmap_id2], widths[mipmap_id2], heights[mipmap_id2], s, t);

	return c1 * (1.0f - mipmap_s) + c2 * mipmap_s;
}

void Texture::SetColor(int u, int v, unsigned int c) {
	if (images.size() == 0) return;
	if (u < 0 || u >= widths[0]) return;
	if (v < 0 || v >= heights[0]) return;

	images[0][u + v * widths[0]] = c;
}

/**
 * Find the nearest two mipmaps according to the width/height of the AABB.
 *
 * @param width		the width of the AABB
 * @param height	the height of the AABB
 * @param ds		the difference between the min and max of the s coordinates of the triangle
 * @param dt		the difference between the min and max of the t coordinates of the triangle
 */
void Texture::SetMipMap(int width, int height, float ds, float dt) {
	if (!scene->mipmap_mode) {
		mipmap_id1 = 0;
		mipmap_id2 = 0;
		return;
	}

	if (ds == 0.0f || dt == 0.0f) {
		mipmap_id1 = 0;
		mipmap_id2 = 0;
		return;
	}

	float w = (float)width / ds;
	float h = (float)height / dt;

	mipmap_id1 = min((int)log2f((float)widths[0] / w), (int)log2f((float)heights[0] / h));

	if (mipmap_id1 < 0) mipmap_id1 = 0;
	if (mipmap_id1 >= images.size()) mipmap_id1 = images.size() - 1;
	mipmap_id2 = mipmap_id1 + 1;
	if (mipmap_id2 >= images.size()) mipmap_id2 = images.size() - 1;

	if (mipmap_id1 == mipmap_id2) {
		mipmap_s = 1.0f;
	} else {
		mipmap_s = (float)(widths[mipmap_id1] - w) / (float)(widths[mipmap_id1] - widths[mipmap_id2]);
	}

	//printf("%d\n", mipmap_id1);
}

/**
 * Create mipmap images.
 *
 * @param width		the width of the original image
 * @param height	the height of the original image
 */
void Texture::CreateMipMap(int width, int height) {
	V3 c0, c1, c2, c3;

	int w = width / 2;
	int h = height / 2;

	while (w >= 1 && h >= 1) {
		unsigned int* image = (unsigned int*)_TIFFmalloc(sizeof(unsigned int) * w * h);
		widths.push_back(w);
		heights.push_back(h);
		images.push_back(image);

		int scaleX = width / w;
		int scaleY = height / h;

		for (int i = 0; i< h; i++) {
			for (int j = 0; j < w; j++) {
				V3 color;

				for (int y = i * scaleY; y < (i + 1) * scaleY; y++) {
					for (int x = j * scaleX; x < (j + 1) * scaleX; x++) {
						V3 c;
						c.SetColor(images[0][x + y * width]);

						color = color + c;
					}
				}

				color = color / (float)scaleX / (float)scaleY;
				image[j + i * w] = color.GetColor();
			}
		}

		w /= 2;
		h /= 2;
	}

	mipmap_id1 = 0;
	mipmap_id2 = 0;
	mipmap_s = 1.0f;
}

void Texture::CreateMipMap(int width, int height, const TIFFImage &src, int x0, int y0) {
	V3 c0, c1, c2, c3;

	int w = width / 2;
	int h = height / 2;

	while (w >= 1 && h >= 1) {
		unsigned int* image = (unsigned int*)_TIFFmalloc(sizeof(unsigned int) * w * h);
		widths.push_back(w);
		heights.push_back(h);
		images.push_back(image);

		int scaleX = width / w;
		int scaleY = height / h;

		//TIFFImage ti(w, h);

		for (int i = 0; i< h; i++) {
			for (int j = 0; j < w; j++) {
				V3 color;

				for (int y = i * scaleY; y < (i + 1) * scaleY; y++) {
					for (int x = j * scaleX; x < (j + 1) * scaleX; x++) {
						V3 c;
						c.SetColor(src.pix[x + x0 + (y + y0) * src.width()]);

						color = color + c;
					}
				}

				color = color / (float)scaleX / (float)scaleY;
				image[j + i * w] = color.GetColor();

				//ti.pix[j + i * w] = color.GetColor();
			}
		}

		//char filename[256];
		//sprintf(filename, "%d_%d_%d.tif", x0, y0, w);
		//ti.save(filename);

		w /= 2;
		h /= 2;
	}

	mipmap_id1 = 0;
	mipmap_id2 = 0;
	mipmap_s = 1.0f;
}

/**
 * Get the color at the texel (u, v) in the specified mipmap image.
 *
 * @param image		the mipmap image
 * @param width		the width of the mipmap image
 * @param height	the height of the mipmap image
 * @param u			the x coordinate of the texel
 * @param v			the y coordinate of the texel
 * @return			the color
 */
V3 Texture::GetColor(unsigned int* image, int width, int height, float u, float v) {
	// locate the corresponding (x, y) in the mipmap image
	float x = (float)(u - (int)u) * (float)width;
	float y = (float)(v - (int)v) * (float)height;
	if (x < 0) x += width;
	if (y < 0) y += height;

	// locte the surrounding 4 texels
	int x0, y0, x1, y1;
	float s, t;
	if (x < 0.5f) {
		x0 = 0;
		s = 1.0f;
	} else {
		x0 = (int)(x - 0.5f);
		s = x - (float)x0 - 0.5f;
	}
	if (y < 0.5f) {
		y0 = 0;
		t = 1.0f;
	} else {
		y0 = (int)(y - 0.5f);
		t = y - (float)y0 - 0.5f;
	}

	x1 = x0 + 1;
	y1 = y0 + 1;
	if (x1 >= width) x1 = width - 1;
	if (y1 >= height) y1 = height - 1;
	
	// get the colors of 4 texels
	V3 c0, c1, c2, c3;
	c0.SetColor(image[x0 + y0 * width]);
	c1.SetColor(image[x1 + y0 * width]);
	c2.SetColor(image[x1 + y1 * width]);
	c3.SetColor(image[x0 + y1 * width]);
	
	return c0 * (1 - s) * (1 - t) + c1 * s * (1 - t) + c2 * s * t  + c3 * (1 - s) * t;
}