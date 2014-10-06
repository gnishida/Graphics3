#pragma once

#include "TIFFImage.h"
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

class SIFT {
private:
	TIFFImage *image;
	double SIG0;
	int S;
	int MINW;
	int OCT;

public:
	SIFT(TIFFImage *image);

private:
	void getKeypoints();
	void localizeKeypoints();
	void downSampling(TIFFImage &src);
};

