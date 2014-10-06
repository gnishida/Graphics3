#include "TIFFImage.h"
#include <iostream>
#include <libtiff/tiffio.h>

TIFFImage::TIFFImage(int w, int h) {
	this->w = w;
	this->h = h;

	pix = (unsigned int*)malloc(sizeof(unsigned int) * w * h);
	for (int i = 0; i < w * h; i++) {
		pix[i] = 0xFF000000;
	}
}

TIFFImage::TIFFImage(char* filename) {
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

int TIFFImage::width() {
	return w;
}

int TIFFImage::height() {
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