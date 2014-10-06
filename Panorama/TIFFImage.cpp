#include "TIFFImage.h"
#include <iostream>
#include <libtiff/tiffio.h>

TIFFImage::TIFFImage() {
	this->w = 0;
	this->h = 0;
	pix = NULL;
}

TIFFImage::TIFFImage(int w, int h) {
	this->w = w;
	this->h = h;

	pix = (unsigned int*)malloc(sizeof(unsigned int) * w * h);
	for (int i = 0; i < w * h; i++) {
		pix[i] = 0xFF000000;
	}
}

TIFFImage::TIFFImage(const char* filename) {
	TIFF* tiff = TIFFOpen(filename, "r");
	if (tiff == NULL)  throw "The TIFF file is not accessible.";

	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &h);

	pix = (unsigned int*)_TIFFmalloc(sizeof(unsigned int) * w * h);
	if (!TIFFReadRGBAImage(tiff, w, h, pix, 0)) {
		delete [] pix;
		TIFFClose(tiff);
		throw "Invalid TIFF file";
	}

	TIFFClose(tiff);
}

int TIFFImage::width() const {
	return w;
}

int TIFFImage::height() const {
	return h;
}

void TIFFImage::reset(int w, int h) {
	delete [] pix;

	this->w = w;
	this->h = h;

	pix = (unsigned int*)malloc(sizeof(unsigned int) * w * h);
	for (int i = 0; i < w * h; i++) {
		pix[i] = 0xFF000000;
	}
}

void TIFFImage::save(const char* filename) {
	unsigned int *temp = new unsigned int[w * h];
	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			temp[v * w + u] = pix[(h - 1 - v) * w + u];
		}
	}

	TIFF* tiff = TIFFOpen(filename, "w");
	if (tiff == NULL) return;

	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, sizeof(unsigned int));
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8);
	//TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_BOTLEFT);
	TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	TIFFWriteEncodedStrip(tiff, 0, temp, w * h * sizeof(unsigned int));

	TIFFClose(tiff);

	delete [] temp;
}