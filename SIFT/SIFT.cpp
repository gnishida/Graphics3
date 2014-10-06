#include "SIFT.h"

SIFT::SIFT(TIFFImage *image) {
	/*
	this->image = image;

	SIG0   = 1.6;
	S      = 3;
	MINW   = 20;

	// define the number of octave
	int W=(src.width()<src.height()) ? src.width():src.height();
    for(OCT=0;W>MINW;OCT++,W/=2);

	// image buffer
	PGM **L=new PGM*[OCT];
    for(int o=0;o<OCT;o++) L[o]=new PGM[S+3];

	getKeypoints(L);
	localizeKeypoints();

	// release image buffer
	for (int i = 0; i < OCT; i++) {
        delete [] L[i];
    }
    delete [] L;
	*/
}



void SIFT::getKeypoints() {
	/*
    // increase factor
    double k = pow(2, 1.0 / (double)S);

	// create smoothed images
    for (int i = 0; i < OCT; i++) {
        double sig = SIG0;
        for (int s = 0; s < S+3; s++) {
			// Gaussian filter for the original image
            L[i][s] = gaussianFilter(src, sig);
            
            sig *= k;
        }
        
        // down sampling
        downSampling();
    }
	*/
}

void SIFT::localizeKeypoints() {

}

void SIFT::downSampling(TIFFImage &src) {
	/*
    int W = src.width() / 2;
    int H = src.height() / 2;

	TIFFImage tmp(src);

	src.reset(W, H);
    
    for (int x = 0; x < W; x++) {
        for (int y = 0; y < H; y++) {
			src.pix[x + y * W] = tmp.pix[2*x + 2*y * tmp.width()];
        }
    }
	*/
}